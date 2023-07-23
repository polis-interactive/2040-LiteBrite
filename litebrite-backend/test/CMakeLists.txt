
cmake_minimum_required(VERSION 3.16)

project(2040_LiteBrite_Tests LANGUAGES CXX C)

include_directories(${root_dir}/third_party/doctest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(
    tests
        main.cpp
        test_infrastructure/test_auth/test_auth.cpp
        test_infrastructure/test_db/test_creation.cpp
        test_infrastructure/test_udp/test_udp_sender.cpp
        test_infrastructure/test_artnet/test_artnet_controller.cpp
)

set(tests_link_libraries pthread auth db udp art_net)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0")
add_definitions(-DTEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(tests ${tests})

target_link_libraries(tests ${tests_link_libraries})
target_include_directories(tests PRIVATE ${internal_dir})
