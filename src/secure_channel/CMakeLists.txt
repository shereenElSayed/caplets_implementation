find_package(OpenSSL REQUIRED)

add_executable(server server.cpp)
target_link_libraries(server PUBLIC OpenSSL::SSL OpenSSL::Crypto)

add_executable(client client.cpp)
target_link_libraries(client PUBLIC botan-lib)