

set(SOURCES db.cpp migrations.cpp seed.cpp user.cpp)
set(TARGET_LIBS ${SQLiteCpp_LINK_LIBRARIES})

add_library(db STATIC ${SOURCES})
target_link_libraries(db PRIVATE ${TARGET_LIBS})