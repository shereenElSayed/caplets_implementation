#include "tests.h"
#include "caplets.h"
#include <string.h>
int main(){


    // Tests::create_tokens_tags("../examples/create_tags.txt");
    CapabilityToken token;
    std::string cap_token = "%FRAME%/home/pi/*+0+111%FRAME%/home/pi/cspot-caplets/*+0+111$FUNCTION:identity_constraint:855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb%FRAME%request:/home/pi/cspot-caplets/laptop:laptop_handler:0:110";
    token.from_string(cap_token, true);
    // token.from_string("E0EAA39F3E798A39E5318A284A0C46E70CC88D73BF2DCC4BB1DBB289463FD58F%FRAME%/home/centos/*+0+111%FRAME%/home/centos/cspot-caplets/*+0+111$FUNCTION:identity_constraint:855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb%FRAME%request:/home/centos/cspot-caplets/log:my_handler:0:110");
    printf("cap token: %s\n\n", token.to_string(true).c_str());
    // return 0;
    IdentityToken id_token;
    // id_token.from_string("23CC6457DB7125BB71C1A85E9FA50F3130C7110346D34AE8D792DBD34CC70AB7%FRAME%DummyBody%FRAME%855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb%FRAME%E0EAA39F3E798A39E5318A284A0C46E70CC88D73BF2DCC4BB1DBB289463FD58F");
    id_token.create_initial_token();
    id_token.add_identity_token("855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb");
    std::string id_token_with_req = id_token.add_request_tag(token.get_tag());
    id_token.from_string(id_token_with_req);
    printf("1Req tag: %s\n", id_token_with_req.c_str());

}