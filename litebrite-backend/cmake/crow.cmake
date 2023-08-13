
set(CROW_LINK_LIBRARIES Crow::Crow)
add_definitions(-DCROW_DISABLE_STATIC_DIR)
find_package(Crow REQUIRED)
message("Found Crow")
