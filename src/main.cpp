#include "tests.h"
#include "caplets.h"
#include <string.h>
int main(){
    // test_capabilitystructure();
    //Tests::test_frame();
    // test_token();
//    Tests::test_signature();
    
    // test_derivation();
    // Tests::test_constraint();

    char val_token[2048];
    strncpy(val_token,"4EB744B4B49783B51C308D1998765EC4B7FD267209F56859412BDEC3EF499D70%FRAME%/home/centos/*+0+111%FRAME%/home/centos/cspot-caplets/*+0+111",sizeof(val_token));
    std::string s = val_token;
    fprintf(stdout, "%s\n", s.c_str());
    Token token;
    token.from_string(s);
    printf("%s", token.to_string_w_tag().c_str());
    bool is_valid = Token::is_valid_token(token);
    fprintf(stdout, "Token is %d\n", is_valid);
    fflush(stdout);
    return 0;
}