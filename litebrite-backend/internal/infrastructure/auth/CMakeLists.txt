

set(SOURCES auth.cpp)
set(TARGET_LIBS ${LIBSODIUM_LIBRARIES} ${OpenSSL_LINK_LIBRARIES})

add_library(auth STATIC ${SOURCES})
target_link_libraries(auth PRIVATE ${TARGET_LIBS})