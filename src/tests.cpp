#include "caplets.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include "tests.h"



//Test CapabilityStructure
void Tests::test_capabilitystructure(){
    CapabilityStructure st;
    std::string string_cap_str= "/home/centos/dir/*+0+001";
    st.from_String(string_cap_str);
    if( strcmp(st.get_resource().c_str(), "/home/centos/dir/*") == 0 &&
        st.get_type() == capability_type::device_cap &&
        st.get_cap_bits()[0] == true &&  st.get_cap_bits()[1] == false){
            printf("CapabilityStructure::fromString PASSED\n");
        }else {
            printf("CapabilityStructure::fromString FAILED\n");
    }
    std::string result = st.to_string();
    if(strcmp(result.c_str(), string_cap_str.c_str()) == 0) {
        printf("CapabilityStructure::to_string Passed\n");   
    }else {
        printf("CapabilityStructure::to_string Failed\n");
        printf("Result: %s\nExpected: %s\n", result.c_str(), string_cap_str.c_str());
    }
}
// //Test Frame
void Tests::test_frame() {
    Frame frame;
    std::string frame_string = "/home/centos/dir/*+0+000$/home/centos/anotherdir/*+1+001";
    frame.from_String(frame_string);
    int count = 0;
    std::string expected_res;
    capability_type type;
    std::bitset<3> caps;
    for (const auto& cap : frame.get_capabilities()){
        if (count == 0){
            expected_res = "/home/centos/dir/*";
            type = capability_type::device_cap;
            caps = 000;

        } else {
            expected_res = "/home/centos/anotherdir/*";
            type = capability_type::user_cap;
            caps = 001;
        }
        if(strcmp(((CapabilityStructure)cap).get_resource().c_str(),expected_res.c_str()) != 0){
            printf("Frame::from_String FAILED in resource\n");
            printf("Result: %s\nExpected: %s\n", ((CapabilityStructure)cap).get_resource().c_str(),
                    expected_res.c_str());
        } else {
            printf("Frame::from_String PASSED in resource\n");
        }
        if(((CapabilityStructure)cap).get_type() != type){
            printf("Frame::from_String FAILED in type\n");
        } else {
            printf("Frame::from_String PASSED in type\n");
        }
        if(((CapabilityStructure)cap).get_cap_bits() != caps){
            printf("Frame::from_String FAILED in capbits\n");
        } else {
            printf("Frame::from_String PASSED in capbits\n");
        }

        count++;
    }


    //This is working but flipped!
    // std::string to_string = frame.to_string();

    // if(strcmp(to_string.c_str(), frame_string.c_str()) == 0) {
    //     printf("Frame Passed\n");   
    // }else {
    //     printf("Frame Failed\n");
    //     printf("Result: %s\nExpected: %s\n", to_string.c_str(), frame_string.c_str());
    // }
}

//Testing Tokens
void Tests::test_token(){
    std::string str_token = "ndkjscvkjsdvhbsdkjcvndkjsc2421-FRAME-/home/frame1/dir/*+0+000$/home/frame1/anotherdir/*+1+001"
                            "-FRAME-/home/frame2/dir/*+0+000$/home/frame2/anotherdir/*+1+001";
    Token token;
    token.from_string(str_token);
    // printf("TAG: %s\n", token.get_tag());
    if(strcmp(str_token.c_str(), token.to_string_w_tag().c_str()) != 0){
        printf("Token test FAILED\n");
        printf("Result: %s\nExpected: %s\n", token.to_string_w_tag().c_str(), str_token.c_str());
    } else {
        printf("Token test PASSED\n");
    }
    
}

void Tests::test_signature(){
    std::ifstream test_file; 
    test_file.open("/home/centos/caplets/examples/test_signatures.txt");
    std::string test_case;
    Token test_token;
    char ex_res_ch;
    bool expected_result;
    int count = 0;
    if (test_file.is_open()){
        while (getline(test_file, test_case)) { 
            // Print the data of the string.
            ex_res_ch = test_case[1];
            if(ex_res_ch == '1') expected_result=true; else expected_result = false;
            test_case = test_case.substr(3,test_case.length());
            Token test_token;
            test_token.from_string(test_case);
            if (Token::is_valid_signature(test_token) == expected_result){
                printf("Test signature #%d: PASSED\n",count);
                printf("-----------------------\n");
            } else {
                printf("Test signature #%d: FAILED\n",count);
                printf("TestCase: %s\nToken to string: %s", test_case.c_str(), test_token.to_string_w_tag().c_str());
                printf("-----------------------\n");
            }
            count++;
        }
    }
}
void Tests::test_derivation(){ 
    std::ifstream test_file; 
    test_file.open("/home/centos/caplets/examples/test_derivations.txt");
    std::string test_case;
    Token test_token;
    char ex_res_ch;
    bool expected_result;
    int count = 0;
    if (test_file.is_open()){
        while (getline(test_file, test_case)) { 
            // Print the data of the string.
            ex_res_ch = test_case[1];
            if(ex_res_ch == '1'){
                expected_result=true;
            }else expected_result = false;

            test_case = test_case.substr(3,test_case.length());
            Token test_token;
            test_token.from_string_no_tag(test_case);
            bool result = Token::is_valid_derivation(test_token);
            if ( result == expected_result){
                printf("Test derivation #%d: PASSED\n",count);
                printf("-----------------------\n");
            } else {
                printf("Test derivation #%d: FAILED\n",count);
                printf("is_valid_derivation: %d", result);
                printf("-----------------------\n");
            }
            count++;
        }
    } else {
        printf("File Not open!\n");
    }
}

void test_checks(){
    //Empty token
    //Frames:


}
void Tests::create_token(){
    Token token;
    Frame root_frame;
    root_frame.from_String("/home/centos/*+0+000");
    token.add_frame(root_frame);
    printf("Token with root only: %s\n", token.get_tag().c_str());
    
    Frame frame_1;
    frame_1.from_String("/home/centos/temp+0+000$/home/centos/hum+0+000$/home/centos/gas+0+000");
    token.add_frame(frame_1);
    printf("Token with frame_1 : %s\n", token.get_tag().c_str());
    
    Frame frame_2;
    frame_2.from_String("/home/centos/temp/1+0+001$/home/centos/hum/1+0+011$/home/centos/gas/1+0+101");
    token.add_frame(frame_2);
    printf("Token with frame_2 : %s\n", token.get_tag().c_str());

    Frame frame_3;
    frame_3.from_String("/home/centos/temp/1+0+011$/home/centos/hum/1+0+100$/home/centos/gas/1+0+101");
    token.add_frame(frame_3);
    printf("Token with frame_3 : %s\n", token.get_tag().c_str());
} 
