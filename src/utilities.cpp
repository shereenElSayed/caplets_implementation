#include <string>
#include <vector>
#include <algorithm>  
#include <utilities.h>
// #include <bits/stdc++.h>
#include <secrets.h>
#include "botan_all.h"


/**
 * splits a string into a vector<string> using the delimeter
 * @param result is the vector with all the strings after the split
 * @param content the string to be split
 * @param delimeter the delimeter to be used to split the string. It can be multiple letters as well
 */
void util::splitString(std::vector<std::string>& result, std::string content, std::string delimeter)
{
    auto prev_pos = content.begin();
    auto next_pos = std::search(prev_pos, content.end(),
                               delimeter.begin(), delimeter.end());
    while (next_pos != content.end())
    {
        result.emplace_back(prev_pos, next_pos);
        prev_pos = next_pos + delimeter.size();
        next_pos = std::search(prev_pos, content.end(), 
                               delimeter.begin(), delimeter.end());
    }

    if (prev_pos != content.end())
    {
        result.emplace_back(prev_pos, content.end());
    }
}


/**
 * Compute  HMAC for a msg with a key
*/
std::string util::compute_mac(const std::string& msg, const std::string& key){
    std::vector<uint8_t> key_vec(key.begin(), key.end());
    Botan::secure_vector<uint8_t> secure_key_vec(key_vec.begin(), key_vec.end());
    return util::compute_mac(msg, secure_key_vec);
}
/**
 * use util::compute_mac(const std::string& msg, const std::string& key) instead
*/
std::string util::compute_mac(const std::string& msg, const Botan::secure_vector<uint8_t>& key) {
   auto hmac = Botan::MessageAuthenticationCode::create_or_throw(ALGORITHM);
   hmac->set_key(key);
   hmac->update(msg);

   return Botan::hex_encode(hmac->final());
}

/**
 * Verification for the capabilities - Path wise
 * @param part Path of a directory in a lower capability 
 * @param full The full path to check if the part param can be reached using the full one
*/
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

/**
 * 
*/
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