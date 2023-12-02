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
    // this->function = const_string.substr(9, func_para_separator_index);
    util::splitString(this->parameters, const_string.substr(func_para_separator_index+1, const_string.size()), ":");
    this->function = this->parameters[0];
    this->parameters.erase(this->parameters.begin());
}

// Frame looks like this: "/home/centos/dir/*+0+000$/home/centos/anotherdir/*+1+001$FUNCTION:<function_name>:<parameter>:<parameter>:<parameter>"
std::string Frame::to_string(){
    std::string result="";
    int count =0, size = this->capabilities.size();
    for (const auto& cap : this->capabilities){
        result+= ((CapabilityStructure)cap).to_string();
        if(count < size-1){
            result += "$";
        }
        count++;
        

    }
    if (this->constraints.size() > 0){
        result += "$";
    }
    count =0;
    size = this->constraints.size();
    for (const auto& constraint : this->constraints){
        result+= ((Constraint)constraint).to_string();
        if(count < size-1){
            result += "$";
        }
        count++;
        

    }
    result.substr(0, result.length() - 1); //remove last $
    return result;
}
void Frame::from_string(const std::string& str){
    std::vector<std::string> string_vec;
    util::splitString(string_vec, str, "$");

    int cap_count = string_vec.size();
    // CapabilityStructure* capstr;
    for(int i=0; i<cap_count; i++){
        int index = string_vec[i].find("FUNCTION:");
        // printf("string: %s with index %d\n",string_vec[i].c_str(), index );
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
    //%FRAME%R:woof_name:handler_name:value:operation
    std::vector<std::string> string_vec;
    util::splitString(string_vec, request, ":", false);

    // printf("Request::from_string:: %s:%s:%s:%s\n", string_vec[1].c_str(), string_vec[2].c_str(), string_vec[3].c_str(), string_vec[4].c_str());
    this->capability.set_resource(string_vec[1]);
    this->handler_name = string_vec[2];
    this->value = string_vec[3];
    this->capability.set_type(capability_type::device_cap);
    this->capability.set_cap_bits(atoi(string_vec[4].c_str()));
}


std::string Request::to_string(){
    return "%FRAME%request:"+this->capability.get_resource() + ":" + this->handler_name + ":" + this->value + ":" + this->capability.get_cap_bits().to_string();
}


// std::string CapabilityToken::to_string_w_tag(){
//     std::string output;
//     output = this->tag + "%FRAME%";
//     int count = this->body.size();
//     for(int i=0; i<count; i++){
//         // printf("CapabilityToken::to_string_w_tag:: %d\n", i);
//         output += this->body[i]->to_string();
//         if(i<count-1){
//             output += "%FRAME%";
//         }
//     }
//     return output;
// }

std::string CapabilityToken::to_string(bool calc_tag){

    std::string output;
    if(calc_tag){
        output = this->get_tag() + "%FRAME%";
    }
    int count = this->body.size();
    for(int i=0; i<count; i++){
        output += this->body[i]->to_string();
        if(i<count){
            output += "%FRAME%";
        }
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

    util::splitString(str_frames, str_token.substr(str_token.find("%FRAME%")+7, str_token.length()), "%FRAME%");    
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
                // add_frame(this->request);
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
    if(this->body.size() == 1){
        //First body- use secret
        tag = util::compute_mac(this->body[0]->to_string(), SECRET);
    } else {
            tag = util::compute_mac(this->body[this->body.size()-1]->to_string(), tag);
    }
        
    if(store_tag){
        this->tag = tag;
    }
    return tag;
}

const bool CapabilityToken::is_valid_signature(){
    std::string calculated_tag=SECRET;
    int body_size = this->body.size();
    for(int i=0; i<body_size; i++){
        calculated_tag = util::compute_mac(this->body[i]->to_string(), calculated_tag);
    }
    if(strcmp(calculated_tag.c_str(), this->tag.c_str())==0){
        // printf("Signature validation passed\n");
        return true;
    } else {
        // printf("Signature validation failed\n");
        return false;
    }
    
}

const bool CapabilityToken::is_valid_derivation(){
    if(this->body.size() == 0) return false;
    if(this->body.size() == 1) return true;
    bool all_cap_are_derived = true;
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

    }
    return true;
}

const bool CapabilityToken::is_valid_constraint() {
    return true;
}

const bool CapabilityToken::is_valid_token(){
    //Signature Verification
    if(this->is_valid_signature() && this->is_valid_derivation() && this->is_valid_constraint()){
        return true;
    }
    return false;
}

const bool CapabilityToken::is_valid_request(){
    int body_size = this->body.size();
    std::vector<CapabilityStructure> last_frame = this->body[body_size-2]->get_capabilities();
    CapabilityStructure request_frame = ((Request*)this->body[body_size-1])->get_capability();
    bool valid_cap = false;
    for(int i=0; i<last_frame.size(); i++){
        if(util::is_path_subset_of_path(
                request_frame.get_resource(),last_frame[i].get_resource())){
            
            if(util::is_cap_subset_of_cap(request_frame ,last_frame[i])){
                    return true;
            } 
        } 
    }
    return false;
}

const bool CapabilityToken::is_valid_request_cspot(const std::bitset<3>& operation_bits){
    if(this->is_valid_request()){
        return util::is_cap_bits_valid(((CapabilityStructure)this->request->get_capability()).get_cap_bits(), operation_bits);
    }
    return false;

}
