# Button Box
add_executable(buttonBox buttonBox/buttonBox.cpp)
target_link_libraries(buttonBox PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Cube
add_executable(cube cube/cube.cpp)
target_link_libraries(cube PRIVATE ${CONAN_LIBS} shaderApi texturesApi)
