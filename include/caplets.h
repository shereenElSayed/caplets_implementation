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
        void set_type(capability_type type){ this->type = type;};
        void set_resource(const std::string& resource){this->resource = resource;};
        void set_cap_bits(const int bits) {this->cap_bits = bits;};
        const std::string to_string();
        void from_string(const std::string cap_string);
};
class Token {
    protected: 
        std::string tag;

    public:
        const std::string get_tag(){return tag;};
        void set_tag(const std::string& tag){this->tag = tag;};
        virtual std::string to_string(bool contain_tag=false)=0;
        virtual void from_string(const std::string& str_token, bool calc_tag=false)=0;

};

class IdentityToken : public Token{
    //<TAG>%FRAME%Dummy%FRAME%<IdentityToken>%FRAME%req_TAG
    private:
        
        std::string identity_token;
        std::string request_tag;
        

    public:
        const std::string get_identity_token(){return identity_token;};
        const std::string get_request_tag(){return request_tag;};
        std::string create_initial_token();
        void add_identity_token(std::string id_token); //add identity token frame then recalculates the tag and stores it.
        std::string add_request_tag(std::string request_tag); //This adds the req tag and calculates the new tag and returns the tag+tokenbody - Does not store tag
        std::string to_string(bool contain_tag=false) override; //Does not adds the request tag frame. Only the tag+dummy frame+identity_token
        void from_string(const std::string& str_token, bool calc_tag=false) override; //Calc_tag is ignored here
        const bool is_valid_signature();
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
        const bool is_valid_constraint(const Token* token=nullptr);

};

// class StaticConstraintFunctions {
//     const static bool identity_constraint(std::string user);
//     const static bool expiration_time(std::string time);
// };
 
class Frame {
    private:
        //Key: the resource
        // Value: vector or capabilities for this resource
        //FUNCTION:<function_name>:<parameter>:<parameter>:<parameter>:FUNCTION:<function_name>:<parameter>:<parameter>
        std::vector<CapabilityStructure> capabilities;
        std::vector<Constraint> constraints;
    public:
        
        virtual std::string to_string();
        virtual void from_string(const std::string& str);
        const std::vector<CapabilityStructure>& get_capabilities(){return capabilities;};
        const std::vector<Constraint>& get_constraints(){return constraints;};
        ~Frame(){printf("IN DESTRUCTOR\n");};
};

class Request : public Frame {
    //%FRAME%RRR:woof_name:handler_name:value:operation
    //example if read: %FRAME%RRR:woof_name:::001
    //example if write with handler: %FRAME%RRR:woof_name:handler_name:value:110
    private:
        CapabilityStructure capability; // Caplability has the resource as the woofname and the bits for the operation
        std::string handler_name;
        std::string value;
    
    public: 
        const CapabilityStructure get_capability(){return capability;};
        const std::string get_handler_name(){return handler_name;};
        const std::string get_value(){return value;};
        void set_capability(const CapabilityStructure & capability){this->capability = capability;};
        void set_handler_name(const std::string& handler_name){this->handler_name = handler_name;};
        void set_value(const std::string& value){this->value = value;};
        
        void from_string(const std::string& str) override;
        std::string to_string() override;
        
};





class CapabilityToken : public Token{
    private:    
        
        std::vector<Frame*> body;
        Request* request=NULL;

        std::string update_tag(bool store_tag=false);
        bool integrety_check();

    public:
        const std::vector<Frame*> get_body(){return body;}
        const Request* get_request(){return request;};
        void set_body_frames(const std::vector<Frame*>& body) { this->body = body;};
        void set_request(Request* request, bool update_tag=false){if (update_tag){this->request = request; this->update_tag(true);} else {this->request = request;} }


        std::string to_string(bool contain_tag=false) override;
        void from_string(const std::string& str_token, bool calc_tag=false) override;
        void from_string_no_tag(const std::string& str_token, bool calc_tag=false);
        
        void add_frame(Frame* frame); //Add the frame to body and recalculate the tag
        const bool is_valid_signature();
        const bool is_valid_derivation(const IdentityToken* token=nullptr);
        const bool is_valid_token(const IdentityToken* token=nullptr);
        const bool is_valid_identity_token(const IdentityToken* token); //attach the identity token
        const bool is_valid_request(const IdentityToken* token); //attach the identity token

        //CSpot specific checks
        const bool is_valid_request_cspot(const IdentityToken* token, const std::bitset<3>& operation_bits);//operation bits are set by the woofput specifiying that this is a put with/without handler
};


