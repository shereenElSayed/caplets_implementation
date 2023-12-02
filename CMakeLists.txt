cmake_minimum_required(VERSION 3.20)

project(caplets LANGUAGES CXX VERSION 0.1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/botan_cmake)

find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})
find_package(Botan 2.19.1 REQUIRED)

botan_generate(
    botan-lib
        system_rng
        sha3
        sha2_32
        auto_rng
        hex
        mac
        rsa

)

add_subdirectory(deps)


add_library(caplets src/caplets.cpp src/utilities.cpp src/tests.cpp)
target_link_libraries(caplets PUBLIC botan-lib ${Boost_LIBRARIES})
target_include_directories(caplets PUBLIC  
                                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                    $<INSTALL_INTERFACE:include> )
                            
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
install(
    TARGETS caplets botan-lib
    EXPORT caplets
    DESTINATION lib
)
install(FILES include/caplets.h DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "capletsConfigVersion.cmake"
    VERSION ${caplets_version}
    COMPATIBILITY AnyNewerVersion)
install(EXPORT caplets
        FILE caplets.cmake
        DESTINATION lib/cmake/caplets)

install(FILES "capletsConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/capletsConfigVersion.cmake"
        DESTINATION lib/cmake/caplets)

add_subdirectory(src)


# install(FILES caplets-config.cmake DESTINATION lib/cmake/caplets)




