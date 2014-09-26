# include
include_directories("$ENV{CODEMETER_PATH}/include" "${WIBUDATA_PATH}/libWibuData")

# src
aux_source_directory(. LIB_SRC)

# target
add_library(ReadWibuData SHARED ${LIB_SRC})

target_link_libraries(ReadWibuData ${WIBU_LIB_PATH})

# install
install(TARGETS ReadWibuData DESTINATION ${INSTALL_DIRECTORY})
