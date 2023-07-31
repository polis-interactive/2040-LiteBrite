
set(SOURCES handlers_auth.cpp webserver.cpp)
set(TARGET_LIBS pthread ${CROW_LINK_LIBRARIES})

add_library(webserver STATIC ${SOURCES})
target_link_libraries(webserver PRIVATE ${TARGET_LIBS})