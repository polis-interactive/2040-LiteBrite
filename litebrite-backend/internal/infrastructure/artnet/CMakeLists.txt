

set(SOURCES universe.cpp controller.cpp artnet.cpp)
set(TARGET_LIBS pthread udp)

add_library(artnet STATIC ${SOURCES})
target_link_libraries(artnet PRIVATE ${TARGET_LIBS})