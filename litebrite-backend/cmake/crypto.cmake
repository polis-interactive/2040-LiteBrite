
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSODIUM REQUIRED libsodium)

# guess I don't really need these
message("Found libsodium")
message("      libsodium include: ${LIBSODIUM_INCLUDE_DIRS}")
message("      libsodium lib: ${LIBSODIUM_LIBRARIES}")

find_package(OpenSSL REQUIRED)
set(OpenSSL_LINK_LIBRARIES ssl crypto)