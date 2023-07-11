
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${root_dir}/bin)

add_definitions(-DAPPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(app main.cpp runtime.cpp)
target_link_libraries(app PRIVATE pthread webserver)