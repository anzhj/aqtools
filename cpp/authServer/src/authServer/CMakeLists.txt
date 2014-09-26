# third party include
include_directories("${LIBUV_PATH}/include")

# include
include_directories("${WIBUDATA_PATH}/libWibuData" "${AUTHORSERVER_PATH}/authServer")

# src
aux_source_directory(. SRC_LISTS)

# add executable file
add_executable(authserver ${SRC_LISTS})

#add_dependencies(authserver ReadWibuData)

# add link library
target_link_libraries(authserver ReadWibuData ${UV_LIB_PATH} pthread ${MD5_LIB_PATH})

# install
install(TARGETS authserver DESTINATION ${INSTALL_DIRECTORY})
