cmake_minimum_required(VERSION 3.20)

project(test-caplets)

set(caplets_DIR "/usr/local/lib/cmake/caplets")
find_package(caplets REQUIRED)

add_executable(test_caplets test_caplets.cpp)
target_link_libraries(test_caplets caplets)
