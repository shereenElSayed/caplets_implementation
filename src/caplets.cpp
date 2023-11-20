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

void CapabilityStructure::from_String(const std::string cap_string) {
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
void Frame::from_String(const std::string str){
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
            capstr.from_String(string_vec[i]);
            this->capabilities.emplace_back(capstr);
        } else {
            Constraint constraint;
            constraint.from_string(string_vec[i]);
            this->constraints.emplace_back(constraint);
        }
        
    }
}


std::string Token::to_string_w_tag(){
    std::string output;
    output = this->tag + "-FRAME-";
    int count = this->body.size();
    for(int i=0; i<count; i++){
        output += this->body[i].to_string();
        if(i<count-1){
            output += "-FRAME-";
        }
    }
    return output;
}

std::string Token::to_string_no_tag(){
    std::string output;
    int count = this->body.size();
    for(int i=0; i<count; i++){
        output += this->body[i].to_string();
        if(i<count){
            output += "-FRAME-";
        }
    }
    return output;
}

void Token::from_string(const std::string& str_token, bool calc_tag){
    //Get tag
    if(!calc_tag) {
        this->tag = str_token.substr(0, str_token.find("-FRAME-",0));
    }
        
    this->from_string_no_tag(str_token, calc_tag);

}
void Token::from_string_no_tag(const std::string& str_token, bool calc_tag){
    
    //Get token body
    std::vector<std::string> str_frames;
    util::splitString(str_frames, str_token.substr(str_token.find("-FRAME-")+7, str_token.length()), "-FRAME-");    
    
    
    for(int i=0; i<str_frames.size();i++){
        Frame frame;
        // printf("frame: %s\n", str_frames[i].c_str());
        frame.from_String(str_frames[i]);
        if(calc_tag)
            add_frame(frame);
        else
            this->body.emplace_back(frame);
    }
}

void Token::add_frame(Frame frame){
    this->body.emplace_back(frame);
    update_tag(true);
}

//Update tag calculates the tag for the last frame added and updates the token tag
std::string Token::update_tag(bool store_tag){
    if(this->body.size() == 1){
        //First body- use secret
        tag = util::compute_mac(this->body[0].to_string(), SECRET);
    } else {
        // std::string str_body = this->to_string_no_tag();
        tag = util::compute_mac(this->body[this->body.size()-1].to_string(), tag);
    }
    if(store_tag){
        this->tag = tag;
    }
    return tag;
}

const bool Token::is_valid_signature(const Token& token){
    std::string calculated_tag=SECRET;
    int body_size = token.body.size();
    for(int i=0; i<body_size; i++){
        calculated_tag = util::compute_mac(((Frame)token.body[i]).to_string(), calculated_tag);
    }
    if(strcmp(calculated_tag.c_str(), token.tag.c_str())==0){
        // printf("Signature validation passed\n");
        return true;
    } else {
        // printf("Signature validation failed\n");
        return false;
    }
    
}

const bool Token::is_valid_derivation(const Token& token){
    if(token.body.size() == 0) return false;
    if(token.body.size() == 1) return true;
    bool all_cap_are_derived = true;
// 
    for(int i=1; i<token.body.size(); i++){
        std::vector<CapabilityStructure> permissive_caps = ((Frame)token.body[i-1]).get_capabilities();
        std::vector<CapabilityStructure> less_permissive_caps = ((Frame)token.body[i]).get_capabilities();
        

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

const bool Token::is_valid_constraint(const Token& token) {
    return true;
}

const bool Token::is_valid_token(const Token& token){
    //Signature Verification
    if(is_valid_signature(token) && is_valid_derivation(token) && is_valid_constraint(token)){
        return true;
    }
    return false;
}