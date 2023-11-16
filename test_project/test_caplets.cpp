#include "caplets.h"

int main() {
    CapabilityStructure st;
    std::string string_cap_str= "/home/centos/dir/*+0+001";
    st.from_String(string_cap_str);
    printf(st.to_string().c_str());
}