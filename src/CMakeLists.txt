# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../botan_cmake)

# find_package(Boost REQUIRED)
# include_directories(${BOOST_INCLUDE_DIRS})
# find_package(Botan 2.19.1 REQUIRED)

# botan_generate(
#     botan-lib
#         system_rng
#         sha3
#         sha2_32
#         auto_rng
#         hex
#         mac
# )

# include(FetchContent)

# FetchContent_Declare(
#   simdjson
#   GIT_REPOSITORY https://github.com/simdjson/simdjson.git
# #   GIT_TAG  tags/v0.9.6
#   GIT_SHALLOW TRUE)

# FetchContent_MakeAvailable(simdjson)



add_executable(main main.cpp)
target_link_libraries(main PUBLIC caplets)
target_include_directories(main PUBLIC "./../include")

add_executable(test run_tests.cpp)
target_link_libraries(test PUBLIC caplets)
# add_subdirectory(secure_channel)