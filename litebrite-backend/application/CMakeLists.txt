
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${root_dir}/bin)

add_executable(embedded embedded.cpp runtime.cpp)
target_link_libraries(embedded PRIVATE pthread service cpp_dotenv)