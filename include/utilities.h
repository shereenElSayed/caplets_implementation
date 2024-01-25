#pragma once
#include <string>
#include <botan_all.h>
#include <caplets.h>

namespace util {
    void splitString(std::vector<std::string>& result, std::string content, std::string delimeter);
    std::string& rtrim(std::string& s, const char* t = "\t\n\r\f\v");
    std::string& ltrim(std::string& s, const char* t= "\t\n\r\f\v");
    std::string& trim(std::string& s, const char* t= "\t\n\r\f\v");
    std::string compute_mac(const std::string& msg, const Botan::secure_vector<uint8_t>& key);
    std::string compute_mac(const std::string& msg, const std::string& key);
    //checks if part (path) is included in full (path)
    bool is_path_subset_of_path(const std::string& part, const std::string& full);
    bool is_cap_subset_of_cap(const CapabilityStructure& part, const CapabilityStructure& full);
    bool is_cap_bits_valid(const std::bitset<3>& part, const std::bitset<3>& full);


}