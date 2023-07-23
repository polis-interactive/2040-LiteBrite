

set(SOURCES universe.cpp controller.cpp art_net.cpp)
set(TARGET_LIBS pthread udp)

add_library(art_net STATIC ${SOURCES})
target_link_libraries(art_net PRIVATE ${TARGET_LIBS})