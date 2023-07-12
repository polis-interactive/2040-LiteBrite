

set(SOURCES sender.cpp)
set(TARGET_LIBS pthread Boost::system Boost::regex)

add_library(udp STATIC ${SOURCES})
target_link_libraries(udp PRIVATE ${TARGET_LIBS})