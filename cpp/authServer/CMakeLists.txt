# cmake version request
cmake_minimum_required(VERSION 2.6)

# project
PROJECT(webservice)

# include
set(AUTHORSERVER_PATH "${PROJECT_SOURCE_DIR}/include")
set(WIBUDATA_PATH "${PROJECT_SOURCE_DIR}/include")

# third party include
set(LIBUV_PATH "${PROJECT_SOURCE_DIR}/depends/libuv-v0.11.19/stage")

# third party library
set(UV_LIB_PATH "${PROJECT_SOURCE_DIR}/depends/libuv-v0.11.19/stage/lib/libuv.a")
set(WIBU_LIB_PATH "/usr/lib/libwibucmlin64.so")
set(MD5_LIB_PATH "/usr/lib/x86_64-linux-gnu/libcrypto.so")

# script
set(START_SCRIPT_PATH "${PROJECT_SOURCE_DIR}/shstart.sh")
set(FIREWALL_SCRIPT_PATH "${PROJECT_SOURCE_DIR}/firewall.sh")

# output
set(INSTALL_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory(src)

#install(PROGRAMS ${START_SCRIPT_PATH} DESTINATION ${INSTALL_DIRECTORY})
#install(PROGRAMS ${FIREWALL_SCRIPT_PATH} DESTINATION ${INSTALL_DIRECTORY})
