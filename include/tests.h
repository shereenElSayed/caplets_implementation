#pragma once
#include <string>

class Tests {
    public:
        static void test_capabilitystructure();
        static void test_frame();
        static void test_token();
        static void test_signature();
        static void test_derivation();
        static void create_token();
        static void test_constraint();
        static void create_tokens_tags(std::string path);
        static void test_requests();
};