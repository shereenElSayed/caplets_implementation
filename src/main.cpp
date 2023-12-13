#include "tests.h"
#include "caplets.h"
#include <string.h>
int main(){


    // Tests::create_tokens_tags("../examples/create_tags.txt");
    CapabilityToken token;
    std::string cap_token = "%FRAME%/home/centos/*+0+111%FRAME%/home/centos/cspot-caplets/*+0+111$FUNCTION:identity_constraint:855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb%FRAME%request:/home/centos/cspot-caplets/log:my_handler:0:110";
    token.from_string(cap_token, true);
    // token.from_string("E0EAA39F3E798A39E5318A284A0C46E70CC88D73BF2DCC4BB1DBB289463FD58F%FRAME%/home/centos/*+0+111%FRAME%/home/centos/cspot-caplets/*+0+111$FUNCTION:identity_constraint:855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb%FRAME%request:/home/centos/cspot-caplets/log:my_handler:0:110");
    printf("cap token: %s\n", token.to_string(true).c_str());
    // return 0;
    IdentityToken id_token;
    // id_token.from_string("23CC6457DB7125BB71C1A85E9FA50F3130C7110346D34AE8D792DBD34CC70AB7%FRAME%DummyBody%FRAME%855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb%FRAME%E0EAA39F3E798A39E5318A284A0C46E70CC88D73BF2DCC4BB1DBB289463FD58F");
    id_token.create_initial_token();
    id_token.add_identity_token("855354fee30548e7c266e5a4a625fb3552a4d1f97050282fb5db8768902793bb");
    printf("1Req tag: %s\n", id_token.add_request_tag(token.get_tag()).c_str());

    // bool valid= token.is_valid_request(&id_token);
    // bool valid = token.is_valid_signature();
    // printf("Valid: %d\n", valid);

    // printf("2Req tag: %s\n", id_token.add_request_tag("AE2981C97D6D7A5DC8F8499B8F41D02CA46C6F563A59E141ECD20EF5D7E6DE9B").c_str());
    // printf("3Req tag: %s\n", id_token.add_request_tag("C66D87CEEE715DED84538942BF8DE3B5E07DE69576131741122FDF0662BA8B60").c_str());
    // printf("4Req tag: %s\n", id_token.add_request_tag("C689FF00EB1442F8DA84364F689AD8CA531E30CBD61C81A93F717FAD21F2F5C3").c_str());
    // printf("5Req tag: %s\n", id_token.add_request_tag("3DF7373D4FF7596FB503D622B1D37A043C320898FD8085816EA8B9584792433D").c_str());
    // printf("6Req tag: %s\n", id_token.add_request_tag("CDB84AD975333426F1BA7861332BD7FDEA92A1050691C8F7B33820F22FE75644").c_str());
    // printf("7Req tag: %s\n", id_token.add_request_tag("94F68C706DA2F8B5B6F7A4C80682510376DCC83189C1281EB93EEE07656630D7").c_str());
    // printf("8Req tag: %s\n", id_token.add_request_tag("4D2CAE28D27321465A7D1E4B37DDE16BB01929A60270699FF266C9E14070415C").c_str());

}