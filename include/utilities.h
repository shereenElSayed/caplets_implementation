#pragma once
#include <string>
#include <botan_all.h>
#include <caplets.h>

namespace util {
    void splitString(std::vector<std::string>& result, const std::string& input, const std::string& sep);
    std::string compute_mac(const std::string& msg, const Botan::secure_vector<uint8_t>& key);
    std::string compute_mac(const std::string& msg, const std::string& key);
    //checks if part (path) is included in full (path)
    bool is_path_subset_of_path(const std::string& part, const std::string& full);
    bool is_cap_subset_of_cap(const CapabilityStructure& part, const CapabilityStructure& full);
    // auto parse_token(const std::string& token);
    // void token_to_string(simdjson::ondemand::document token, std::string& token_str);

}