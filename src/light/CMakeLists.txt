# Color
add_executable(color color/color.cpp)
target_link_libraries(color PRIVATE ${CONAN_LIBS} shaderApi cameraApi)