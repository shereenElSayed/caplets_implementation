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

// enum device_capability {
//     all,
//     Write_W_Handler_Read,
//     Write_No_Handler_Read,
//     Write_W_Handler,
//     Write_No_Handler,
//     Read_Only
// };


class CapabilityStructure {
    private:
        std::string resource;
        capability_type type;

        std::bitset<3> cap_bits;
        //Device Cap code: 0: *
        //                 1: Write_W_Handler_Read
        //                 2: Write_No_Handler_Read
        //                 3: Write_W_Handler
        //                 4: Write_No_Handler
        //                 5: Read_Only
    public: 
        const capability_type get_type(){return this->type;};
        const std::bitset<3> get_cap_bits(){return this->cap_bits;};
        const std::string get_resource(){return this->resource;};
        const std::string to_string();
        void from_String(const std::string cap_string);
};

class Frame {
    private:
        //Key: the resource
        //Value: vector or capabilities for this resource
        std::vector<CapabilityStructure> capabilities;
    public:
        //vector<constraint> constraints;
        std::string to_string();
        void from_String(const std::string str);
        const std::vector<CapabilityStructure>& get_capabilities(){return capabilities;};
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
        void from_string(const std::string& str_token);
        void from_string_no_tag(const std::string& str_token);
        void add_frame(Frame frame); //Add the frame to body and recalculate the tag
        const static bool is_valid_signature(const Token& token);
        const static bool is_valid_derivation(const Token& token);
        const static bool is_valid_constraint(const Token& token);
        const static bool is_valid_token(const Token& token); 
};



class Verifications {
    public:
        static bool is_valid_token(std::string str_token);

};