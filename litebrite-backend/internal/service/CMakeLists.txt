
set(SOURCES embedded_service.cpp)
set(TARGET_LIBS art_net db graphics webserver)

add_library(service STATIC ${SOURCES})
target_link_libraries(service PRIVATE ${TARGET_LIBS})