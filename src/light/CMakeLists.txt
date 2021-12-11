# Color
add_executable(color color/color.cpp)
target_link_libraries(color PRIVATE ${CONAN_LIBS} shaderApi cameraApi)

# Phong Light. Ambient
add_executable(phongLightningAmbient phongLightning/ambient/ambient.cpp)
target_link_libraries(phongLightningAmbient PRIVATE ${CONAN_LIBS} shaderApi cameraApi)

# Phong Light. Diffuse
add_executable(phongLightningDiffuse phongLightning/diffuse/diffuse.cpp)
target_link_libraries(phongLightningDiffuse PRIVATE ${CONAN_LIBS} shaderApi cameraApi)

# Phong Light. Specular
add_executable(phongLightningSpecular phongLightning/specular/specular.cpp)
target_link_libraries(phongLightningSpecular PRIVATE ${CONAN_LIBS} shaderApi cameraApi)

# Phong Full
add_executable(phongLightningFull phongLightning/full/full.cpp)
target_link_libraries(phongLightningFull PRIVATE ${CONAN_LIBS} shaderApi cameraApi)
