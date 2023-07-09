
set(SQLITEORM_LINK_LIBRARIES sqlite_orm::sqlite_orm)
find_package(SqliteOrm REQUIRED)
message("Found SqliteOrm")