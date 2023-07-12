
set(CROW_LINK_LIBRARIES Crow::Crow ZLIB::ZLIB)
add_definitions(-DCROW_ENABLE_COMPRESSION)
add_definitions(-DCROW_DISABLE_STATIC_DIR)
find_package(Crow REQUIRED)
find_package(ZLIB REQUIRED)
message("Found Crow")
