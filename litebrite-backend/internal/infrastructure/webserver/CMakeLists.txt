
set(SOURCES webserver.cpp route_handlers.cpp)
set(TARGET_LIBS pthread ${CROW_LINK_LIBRARIES} ${LIBSODIUM_LIBRARIES} ${OpenSSL_LINK_LIBRARIES})

add_library(webserver STATIC ${SOURCES})
target_link_libraries(webserver PRIVATE ${TARGET_LIBS})