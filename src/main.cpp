#include "tests.h"
#include "caplets.h"
#include <string.h>
int main(){

    // Tests::test_capabilitystructure();
    // Tests::test_frame();
    // Tests::test_token();
    // Tests::test_signature();
    // Tests::test_derivation();
    // Tests::create_tokens_tags("/home/centos/caplets/examples/test_requests.txt");
    // Tests::test_requests();
    CapabilityToken token;
    token.from_string("EC8B93B15AF6B703551FA89B97C4543A212E3927AE271AC584AAD53FC6E956D8%FRAME%/home/centos/*+0+111%FRAME%%FRAME%request:/home/centos/woof:my_handler:1:110");
    bool valid = token.is_valid_request_cspot( 1);
    printf("%d\n", valid);
    return 0;
}