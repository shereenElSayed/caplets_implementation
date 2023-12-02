#include <string>
#include <vector>
#include <algorithm>  
#include <utilities.h>
// #include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <secrets.h>
#include "botan_all.h"

// using namespace util;

void util::splitString(std::vector<std::string>& result, const std::string& input, const std::string& sep, bool token_compress_on) {
    if(token_compress_on)
        boost::split(result, input, boost::is_any_of(sep), boost::token_compress_on);
    else
        boost::split(result, input, boost::is_any_of(sep));
}

std::string util::compute_mac(const std::string& msg, const std::string& key){
    std::vector<uint8_t> key_vec(key.begin(), key.end());
    Botan::secure_vector<uint8_t> secure_key_vec(key_vec.begin(), key_vec.end());
    return util::compute_mac(msg, secure_key_vec);
}

std::string util::compute_mac(const std::string& msg, const Botan::secure_vector<uint8_t>& key) {
   auto hmac = Botan::MessageAuthenticationCode::create_or_throw(ALGORITHM);
   hmac->set_key(key);
   hmac->update(msg);

   return Botan::hex_encode(hmac->final());
}

bool util::is_path_subset_of_path(const std::string& part, const std::string& full){
    std::vector<std::string> part_vec, full_vec;
    util::splitString(part_vec, part, "/");
    util::splitString(full_vec, full, "/");
    int part_vec_size = part_vec.size();
    int full_vec_size = full_vec.size();
    for(int i=0; i<part_vec_size; i++){
        if(i<full_vec_size){
            if(strcmp("*", full_vec[i].c_str()) == 0) continue;
            // printf("Comparing: %s and %s", part_vec[i].c_str(), full_vec[i].c_str());
            // printf("RES %d\n", strcmp(part_vec[i].c_str(), full_vec[i].c_str()));
            if(strcmp(part_vec[i].c_str(), full_vec[i].c_str()) != 0){
                return false;
            }
        }
    }
    return true;
}
template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N-1; i >= 0; i--) {
        if (x[i] ^ y[i]) return y[i];
    }
    return false;
}

bool util::is_cap_subset_of_cap(const CapabilityStructure& part, const CapabilityStructure& full){
    if(((CapabilityStructure)part).get_type() != ((CapabilityStructure)full).get_type()){
        return false;
    }
    return is_cap_bits_valid(((CapabilityStructure)part).get_cap_bits(), ((CapabilityStructure)full).get_cap_bits());

}
bool util::is_cap_bits_valid(const std::bitset<3>& part, const std::bitset<3>& full){
    if( (part & full) == part){
        return true;
    }
    return false;
}