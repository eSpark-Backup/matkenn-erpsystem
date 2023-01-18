set(SOURCES src/qrscanner.cpp)

idf_component_register( SRCS ${SOURCES}
                        INCLUDE_DIRS "include"
                        REQUIRES quirc Camera 
                        PRIV_REQUIRES esp32-camera)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wdeprecated-declarations)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wcpp)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-write-strings)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-function)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)