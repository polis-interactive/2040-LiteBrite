
set(SOURCES graphics.cpp)
set(TARGET_LIBS pthread)

add_library(graphics STATIC ${SOURCES})
target_link_libraries(graphics PRIVATE ${TARGET_LIBS})