

set(SOURCES db.cpp migrations.cpp seed.cpp site.cpp user.cpp display.cpp)
set(TARGET_LIBS ${SQLiteCpp_LINK_LIBRARIES})

add_library(db STATIC ${SOURCES})
target_link_libraries(db PRIVATE ${TARGET_LIBS})