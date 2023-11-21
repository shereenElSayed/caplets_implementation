#pragma once
#include <vector> 
#include <string>
#include <bitset>
#include <map>
// using namespace std;


enum capability_type {
    device_cap,
    user_cap,
    owner_cap
};

class CapabilityStructure {
    private:
        std::string resource;
        capability_type type;
        
        //Device Cap code: 3 bits Handler-Write-Read
        //i.e.: write with handler and read --> 111
        //      write and read but no handler --> 011
        //      write no handler --> 010
        //      read only --> 001         
        std::bitset<3> cap_bits;
        
    public: 
        const capability_type get_type(){return this->type;};
        const std::bitset<3> get_cap_bits(){return this->cap_bits;};
        const std::string get_resource(){return this->resource;};
        const std::string to_string();
        void from_String(const std::string cap_string);
};

class Constraint {
    
    private:
        std::string function;
        std::vector<std::string> parameters;
    public:
        const std::string get_function(){return this->function;};
        const std::vector<std::string> get_parameters(){return this->parameters;};
        const std::string to_string();
        void from_string(const std::string const_string);
};

class StaticConstraintFunctions {
    const static bool identity_constraint(std::string user);
    const static bool expiration_time(std::string time);
};
 
class Frame {
    private:
        //Key: the resource
        // Value: vector or capabilities for this resource
        //FUNCTION:<function_name>:<parameter>:<parameter>:<parameter>:FUNCTION:<function_name>:<parameter>:<parameter>
        std::vector<CapabilityStructure> capabilities;
        std::vector<Constraint> constraints;
    public:
        
        std::string to_string();
        void from_String(const std::string str);
        const std::vector<CapabilityStructure>& get_capabilities(){return capabilities;};
        const std::vector<Constraint>& get_constraints(){return constraints;};
};

class Request {
    private:
        std::string woof_name;
        std::bitset<3> operation;
        std::string handler_name;
        std::string value;
    
    public: 
        const std::string get_woof_name(){return woof_name;};
        const std::string get_handler_name(){return handler_name;};
        const std::string get_value(){return value;};
        
};

class Token {
    private:    
        std::string update_tag(bool store_tag=false);
        bool integrety_check();
        std::string tag;
        std::vector<Frame> body;

    public:
        const std::string get_tag(){return tag;};
        const std::vector<Frame> get_body(){return body;}
        std::string to_string_w_tag();
        std::string to_string_no_tag();
        void from_string(const std::string& str_token, bool calc_tag=false);
        void from_string_no_tag(const std::string& str_token, bool calc_tag=false);
        void add_frame(Frame frame); //Add the frame to body and recalculate the tag
        const static bool is_valid_signature(const Token& token);
        const static bool is_valid_derivation(const Token& token);
        const static bool is_valid_constraint(const Token& token);
        const static bool is_valid_token(const Token& token);

};


