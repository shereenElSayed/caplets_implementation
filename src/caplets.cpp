#include "caplets.h"
#include <iostream>
#include <stdio.h>
#include <utilities.h>
#include <cstring>
#include <secrets.h>


// Capability looks like this: "0+000" this means 0 for device and 000 is the capability
const std::string CapabilityStructure::to_string(){
    return this->resource + "+" + std::to_string(this->type) + "+" +this->cap_bits.to_string();
}

void CapabilityStructure::from_string(const std::string cap_string) {
    std::vector<std::string> result;
    util::splitString(result, cap_string, "+");
    
    this->resource = result[0];
    // capability_structure cpstr;

    if (strcmp(result[1].c_str(), "0") == 0){
        this->type = capability_type::device_cap;
    } else if (strcmp(result[1].c_str(), "1") == 0){
        this->type = capability_type::user_cap;
    }
    this->cap_bits = atoi(result[2].c_str());
}


const std::string Constraint::to_string(){
    std::string result =  "FUNCTION:" + this->function;
    for(std::string s : this->parameters){
        result += ":" + s;
    }
    return result;
}

void Constraint::from_string(const std::string const_string){
    //FUNCTION:<function_name>:<parameter>:<parameter>:<parameter>
    int func_para_separator_index = const_string.find(':');
    util::splitString(this->parameters, const_string.substr(func_para_separator_index+1, const_string.size()), ":");
    this->function = this->parameters[0];

    this->parameters.erase(this->parameters.begin());
}

// Frame looks like this: "/home/centos/dir/*+0+000$/home/centos/anotherdir/*+1+001$FUNCTION:<function_name>:<parameter>:<parameter>:<parameter>"
std::string Frame::to_string(){
    std::string result="";
    int count =0, size = this->capabilities.size();
    for (const auto cap : this->capabilities){
        result+= ((CapabilityStructure)cap).to_string();
        if(count < size-1){
            result += "$";
        }
        count++;
        

    }

    count =0;
    size = this->constraints.size();
    for (const auto constraint : this->constraints){
        result+= "$" + ((Constraint)constraint).to_string();
        // if(count < size-1){
        //     result += "$";
        // }
        // count++;
        

    }
    result.substr(0, result.length() - 1); //remove last $
    return result;
}
void Frame::from_string(const std::string& str){
    std::vector<std::string> string_vec;
    util::splitString(string_vec, str, "$");

    int cap_count = string_vec.size();

    for(int i=0; i<cap_count; i++){
        int index = string_vec[i].find("FUNCTION:");
        // printf("FRAME:: string: %s with index %d\n",string_vec[i].c_str(), index );
        if (index == -1) {
            CapabilityStructure capstr;
            // printf("Frame:: from string: %s\n",string_vec[i].c_str());
            capstr.from_string(string_vec[i]);
            this->capabilities.emplace_back(capstr);
        } else {
            Constraint constraint;
            constraint.from_string(string_vec[i]);
            this->constraints.emplace_back(constraint);
        }
        
    }
}

void Request::from_string(const std::string& request){
    //request:woof_name:handler_name:value:operation
    std::vector<std::string> string_vec;
    util::splitString(string_vec, request, ":");

    // printf("Request::from_string:: %s:%s:%s:%s\n", string_vec[1].c_str(), string_vec[2].c_str(), string_vec[3].c_str(), string_vec[4].c_str());
    this->capability.set_resource(string_vec[1]);
    this->handler_name = string_vec[2];
    this->value = string_vec[3];
    this->capability.set_type(capability_type::device_cap);
    this->capability.set_cap_bits(atoi(string_vec[4].c_str()));
}


std::string Request::to_string(){
    return "request:"+this->capability.get_resource() + ":" + this->handler_name + ":" + this->value + ":" + this->capability.get_cap_bits().to_string();
}

std::string CapabilityToken::to_string(bool calc_tag){

    std::string output;
    if(calc_tag){
        output = this->get_tag() ;
    }
    
    int count = this->body.size();
    for(int i=0; i<count; i++){
        output +=  "%FRAME%";
        output += this->body[i]->to_string();
        // if(i<count-1){
        //     output += "%FRAME%";
        // }
    }
    return output;
}

void CapabilityToken::from_string(const std::string& str_token, bool calc_tag){
    //Get tag
    if(!calc_tag) {
        this->set_tag(str_token.substr(0, str_token.find("%FRAME%",0)));
    }
        
    this->from_string_no_tag(str_token, calc_tag);

}
void CapabilityToken::from_string_no_tag(const std::string& str_token, bool calc_tag){
    
    //Get token body
    std::vector<std::string> str_frames;
    bool has_request = false;
    // printf(str_token.substr(str_token.find("%FRAME%")+7, str_token.length()))
    util::splitString(str_frames, str_token.substr(str_token.find("%FRAME%")+7, str_token.length()), "%FRAME%");
    // for(auto s : str_frames){
    //     printf("from_string_no_tag:: frame: %s\n", s.c_str());
    // }

    int index_of_request = str_frames[str_frames.size()-1].find("request:");
    // printf("index:%d\n", index_of_request);
    if (index_of_request >= 0){
        has_request = true;
        // printf("true\n");
    }
    // printf("size of frames: %d\n", str_frames.size());
    for(int i=0; i<str_frames.size();i++){
        // printf("i: %d\n", i);
        if((has_request && i!=str_frames.size()-1) || !has_request){
            Frame* frame = new Frame();
            // printf("frame: %s\n", str_frames[i].c_str());
            frame->from_string(str_frames[i]);
            
            if(calc_tag)
                add_frame(frame);
            else
                this->body.emplace_back(frame);
        }
        else {
            this->request = new Request();
            this->request->from_string(str_frames[i]);

            // printf("Request :: to_string:: %s\n", this->request->to_string().c_str());
            if(calc_tag){
                this->body.emplace_back(this->request);
                update_tag(true);
                // printf("Size of list %d\n", this->body.size());
            }
            else
                this->body.emplace_back(this->request);
        }
    }

}

void CapabilityToken::add_frame(Frame* frame){
    this->body.emplace_back(frame);
    update_tag(true);
}

//Update tag calculates the tag for the last frame added and updates the token tag
std::string CapabilityToken::update_tag(bool store_tag){
    std::string new_tag;
    if(this->body.size() == 1){
        //First body- use secret
        new_tag = util::compute_mac(this->body[0]->to_string(), SECRET);
    } else {
        new_tag = util::compute_mac(this->body.back()->to_string(), this->tag);
    }
        
    if(store_tag){
        this->tag = new_tag;
    }
    return new_tag;
}

const bool CapabilityToken::is_valid_signature(){
    std::string calculated_tag=SECRET;
    int body_size = this->body.size();
    for(int i=0; i<body_size; i++){
        // printf("frame: %s\n", this->body[i]->to_string().c_str());
        calculated_tag = util::compute_mac(this->body[i]->to_string(), calculated_tag);
    }
    if(strcmp(calculated_tag.c_str(), this->tag.c_str())==0){
        printf("Signature validation passed\n");
        return true;
    } else {
        printf("Signature validation failed\n");
        printf("Calculated tag: %s\nOriginal tag: %s\n",calculated_tag.c_str(),  this->tag.c_str());
        return false;
    }
    
}

const bool CapabilityToken::is_valid_derivation(const IdentityToken* token)//this token is identity_token
{
    if(this->body.size() == 0) return false;
    if(this->body.size() == 1) return true;
    bool all_cap_are_derived = true, identity_constraint_checked = false;
    int size = this->body.size();
    if (this->request != NULL){
        size -=1;
    }
// 
    for(int i=1; i<this->body.size(); i++){
        std::vector<CapabilityStructure> permissive_caps = this->body[i-1]->get_capabilities();
        std::vector<CapabilityStructure> less_permissive_caps = this->body[i]->get_capabilities();
        

        for(int j=0; j<less_permissive_caps.size(); j++){
            bool valid_cap = false;
            for(int z=0; z<permissive_caps.size(); z++){
                
                if(util::is_path_subset_of_path(
                    less_permissive_caps[j].get_resource(),permissive_caps[z].get_resource())){
                        
                        if(util::is_cap_subset_of_cap(less_permissive_caps[j],permissive_caps[z])){
                                valid_cap = true;
                                break;
                        } else {
                            printf("Failed for cap check %s and %s\n",less_permissive_caps[j].to_string().c_str(),
                             permissive_caps[z].to_string().c_str());
                        }
                } else {
                        printf("Failed for path check %s and %s\n",less_permissive_caps[j].get_resource().c_str(),
                         permissive_caps[z].get_resource().c_str());
                }
            }
            if(!valid_cap) return false;
        }

        //Check if there is constraint -- if yes, call is_valid_constraint
        //Constraints for expiration time should be checked here
        //Constraints for identity check
        if(!identity_constraint_checked && this->body[i-1]->get_constraints().size() > 0){
            // printf("checking constraint\n");
            identity_constraint_checked = true;
            for(auto constraint : this->body[i-1]->get_constraints()){
                if(!constraint.is_valid_constraint(token)){
                    return false;
                }
            }
        }

    }
    return true;
}

const bool Constraint::is_valid_constraint(const Token* token) {
    if(strcmp(this->get_function().c_str(), "identity_constraint") == 0){
        if(token == nullptr || this->get_parameters().size() != 1){
            return false;
        }
        IdentityToken* id_token = (IdentityToken*)token;
        if(strcmp(id_token->get_identity_token().c_str(), this->get_parameters()[0].c_str())==0){
            return true;
        }
        return false;
    } else if(strcmp(this->get_function().c_str(), "expiration_time") == 0){
        //TODO
    }

    return true;
}

const bool CapabilityToken::is_valid_token(const IdentityToken* token){
    //Signature Verification
    if(this->is_valid_signature() && this->is_valid_derivation(token)){
        return true;
    }
    return false;
}

//Verifies derivation and signature for token
const bool CapabilityToken::is_valid_identity_token(const IdentityToken* token){

    IdentityToken* id_token = (IdentityToken*)token;
    if(id_token->get_identity_token().length() == 0 || id_token->get_request_tag().length() == 0 || id_token->get_tag().length() == 0) {
        printf("is_valid_identity_token:: false 1\n");
        return false;
    }

    if(strcmp(id_token->get_request_tag().c_str(), this->get_tag().c_str()) != 0){
        // printf("id_token->get_request_tag(): %s\nthis->get_tag(): %s\n", id_token->get_request_tag().c_str(), this->get_tag().c_str());
        printf("is_valid_identity_token:: false 2\n");
        return false;
    }

    if(!id_token->is_valid_signature()){
        printf("is_valid_identity_token:: false 3\n");
        return false;
    }
    
    // printf("is_valid_identity_token:: True\n");
    return true;


}

const bool CapabilityToken::is_valid_request(const IdentityToken* token){
    bool continue_validation = false;
    if(token == nullptr){
        // printf("token is nullptr\n");
        //maybe there is no identity_constraint,, we need to check without it too
        if(this->is_valid_token(token))
            continue_validation = true;
    } else if(this->is_valid_identity_token(token) && this->is_valid_token(token)){
        continue_validation = true;
        printf("continue_validation\n");
    }

    if(continue_validation){

        int body_size = this->body.size();
        std::vector<CapabilityStructure> last_frame = this->body[body_size-2]->get_capabilities();
        CapabilityStructure request_frame = ((Request*)this->body[body_size-1])->get_capability();
        bool valid_cap = false;
        for(int i=0; i<last_frame.size(); i++){
            if(util::is_path_subset_of_path(request_frame.get_resource(),last_frame[i].get_resource())){
                
                if(util::is_cap_subset_of_cap(request_frame ,last_frame[i])){
                        return true;
                } 
            } 
        }
    }
    
    return false;
}

const bool CapabilityToken::is_valid_request_cspot(const IdentityToken* token, const std::bitset<3>& operation_bits){
    if(is_valid_request(token)){
        printf("token is valid, checking the bits now\n");
        return util::is_cap_bits_valid(((CapabilityStructure)this->request->get_capability()).get_cap_bits(), operation_bits);
    }
    return false;

}

std::string IdentityToken::to_string(bool contain_tag){
    std::string token="";
    if(contain_tag)
        token = tag;

    token += "%FRAME%" + IDENTITY_TOKEN_DUMMY_BODY;
    if (!(identity_token.length() == 0 || identity_token.empty())) {
        token += "%FRAME%" + identity_token;
    }
    // if (!(request_tag.length() == 0 || request_tag.empty())) {
    //     token += "%FRAME%" + request_tag;
    // }
    return token;
}



void IdentityToken::from_string(const std::string& str_token, bool calc_tag){
    this->tag = str_token.substr(0, str_token.find("%FRAME%",0));
    std::vector<std::string> str_frames;

    util::splitString(str_frames, str_token.substr(str_token.find("%FRAME%")+7, str_token.length()), "%FRAME%");
    for(int i=1; i<str_frames.size(); i++) {
        if(i==1){
            this->identity_token = str_frames[i];
        } else if(i==2){
            this->request_tag = str_frames[i];
        }
        
    }

}


void IdentityToken::add_identity_token(std::string id_token){
    this->identity_token = id_token;
    this->tag = util::compute_mac(id_token, this->tag);
}

std::string IdentityToken::add_request_tag(std::string request_tag){
    std::string new_tag = util::compute_mac(request_tag, this->tag);
    return new_tag + "%FRAME%"+ IDENTITY_TOKEN_DUMMY_BODY +"%FRAME%" + this->identity_token + "%FRAME%" + request_tag;
}

std::string IdentityToken::create_initial_token(){
    this->tag = util::compute_mac(IDENTITY_TOKEN_DUMMY_BODY, SECRET);
    return tag + "%FRAME%" + IDENTITY_TOKEN_DUMMY_BODY;
}

const bool IdentityToken::is_valid_signature(){
    //Signature verification
    std::string new_tag = util::compute_mac(IDENTITY_TOKEN_DUMMY_BODY, SECRET);
    new_tag = util::compute_mac(this->identity_token, new_tag);
    new_tag = util::compute_mac(this->request_tag, new_tag);
    if(strcmp(new_tag.c_str(), this->tag.c_str()) == 0){
        return true;
    }
    return false;
}