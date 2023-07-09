cmake_minimum_required(VERSION 3.16)


set(CMAKE_VERBOSE_MAKEFILE ON)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    message("Setting debug flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")


# pre-project cmake files

project(
    2040_LiteBrite
    VERSION 1.0
    LANGUAGES C CXX
)

# post-project cmake files
include(cmake/boost.cmake)
include(cmake/crow.cmake)
include(cmake/crypto.cmake)
include(cmake/sqlite_orm.cmake)

set(root_dir ${CMAKE_CURRENT_LIST_DIR})
set(internal_dir ${root_dir}/internal)
set(third_party_dir ${root_dir}/third_party)

include_directories(${root_dir}/internal)
add_definitions(-DJWT_DISABLE_PICOJSON=1)
include_directories(${third_party_dir})

add_subdirectory(${internal_dir}/infrastructure)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_subdirectory(${root_dir}/test)
endif()

add_subdirectory(${root_dir}/application)
