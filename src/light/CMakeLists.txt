# Color
add_executable(color color/color.cpp)
target_link_libraries(color PRIVATE ${CONAN_LIBS} shaderApi cameraApi)

# Phong Light. Ambient
add_executable(phongLightningAmbient phongLightning/ambient/ambient.cpp)
target_link_libraries(phongLightningAmbient PRIVATE ${CONAN_LIBS} shaderApi cameraApi)
