set(SOURCES src/cloud.cpp src/firebase.cpp)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "include"
                    REQUIRES esp_firebase jsoncpp 
                    PRIV_REQUIRES esp_http_client esp-tls)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wdeprecated-declarations)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wcpp)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)