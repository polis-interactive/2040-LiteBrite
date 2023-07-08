
set(OATPP_VERSION 1.3.0)
set(OATPP_LINK_LIBRARIES oatpp::oatpp)  # use public
set(OATPP_TEST_LINK_LIBRARIES oatpp::oatpp-test)  # use public

find_package(oatpp ${OATPP_VERSION} REQUIRED)
message(STATUS "libcamera library found:")
message(STATUS "    version: ${OATPP_VERSION}")
message(STATUS "    libraries: ${OATPP_LINK_LIBRARIES}")
message(STATUS "    test libraries: ${OATPP_TEST_LINK_LIBRARIES}")