# Button Box
add_executable(buttonBox buttonBox/buttonBox.cpp)
target_link_libraries(buttonBox PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Color cube
add_executable(colorCube cube/colorCube.cpp)
target_link_libraries(colorCube PRIVATE ${CONAN_LIBS} shaderApi)

# Texture cube
add_executable(textureCube cube/textureCube.cpp)
target_link_libraries(textureCube PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Cubes
add_executable(cubes cubes/cubes.cpp)
target_link_libraries(cubes PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Round Camera
add_executable(roundCamera roundCamera/roundCamera.cpp)
target_link_libraries(roundCamera PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Camera WASD
add_executable(cameraWASD cameraWASD/cameraWASD.cpp)
target_link_libraries(cameraWASD PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Camera mouse
add_executable(cameraMouse cameraMouse/cameraMouse.cpp)
target_link_libraries(cameraMouse PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Color cube camera
add_executable(colorCubeCamera colorCubeCamera/colorCubeCamera.cpp)
target_link_libraries(colorCubeCamera PRIVATE ${CONAN_LIBS} shaderApi cameraApi)
