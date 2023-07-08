
set(BOOST_ROOT "/usr/local/lib/boost_1_81_0")


set(BOOST_INCLUDEDIR  "${BOOST_ROOT}")

set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
set(Boost_Version 1.81.0)

set(Boost_NO_WARN_NEW_VERSIONS 1)
find_package(Boost ${Boost_Version} REQUIRED COMPONENTS system regex)
include_directories(${BOOST_INCLUDEDIR})
