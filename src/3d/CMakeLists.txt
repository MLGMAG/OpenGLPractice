# Button Box
add_executable(buttonBox buttonBox/buttonBox.cpp)
target_link_libraries(buttonBox PRIVATE ${CONAN_LIBS} shaderApi texturesApi)
