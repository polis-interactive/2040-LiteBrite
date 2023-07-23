
set(SOURCES embedded.cpp)
set(TARGET_LIBS pthread)

add_library(service STATIC ${SOURCES})
target_link_libraries(service PRIVATE ${TARGET_LIBS})