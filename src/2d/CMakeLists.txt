add_executable(window window/window.cpp)
target_link_libraries(window ${CONAN_LIBS})

add_executable(legacyTriangle legacyTriangle/legacyTriangle.cpp)
target_link_libraries(legacyTriangle ${CONAN_LIBS})

add_executable(initGlew initGlew/initGlew.cpp)
target_link_libraries(initGlew ${CONAN_LIBS})

add_executable(glVersion glVersion/glVersion.cpp)
target_link_libraries(glVersion ${CONAN_LIBS})

add_executable(triangle triangle/triangle.cpp)
target_link_libraries(triangle PRIVATE ${CONAN_LIBS} shaderApi)

add_executable(square square/square.cpp)
target_link_libraries(square PRIVATE ${CONAN_LIBS} shaderApi)

add_subdirectory(catInOrange)

# Vertex Array Object Practice
add_executable(vertexArrayObjectPractice vertexArrayObjectPractice/vertexArrayObjectPractice.cpp)
target_link_libraries(vertexArrayObjectPractice PRIVATE ${CONAN_LIBS} shaderApi)

# Element Buffer Object Practice
add_executable(elementBufferObjectPractice elementBufferObjectPractice/elementBufferObjectPractice.cpp)
target_link_libraries(elementBufferObjectPractice PRIVATE ${CONAN_LIBS} shaderApi)

# Uniform Practice
add_executable(uniform uniform/uniform.cpp)
target_link_libraries(uniform PRIVATE ${CONAN_LIBS} shaderApi)

# Layout Practice
add_executable(layout layout/layout.cpp)
target_link_libraries(layout PRIVATE ${CONAN_LIBS} shaderApi)

# Inverted Triangle
add_executable(invertedTriangle invertedTriangle/invertedTriangle.cpp)
target_link_libraries(invertedTriangle PRIVATE ${CONAN_LIBS} shaderApi)

# Texture triangle
add_executable(textureBox textureBox/textureBox.cpp)
target_link_libraries(textureBox PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Matrix Math
add_executable(matrixMath matrixMath/matrixMath.cpp)
target_link_libraries(matrixMath PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Transformation Practice
add_executable(transformation transformation/transformation.cpp)
target_link_libraries(transformation PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

# Plot
add_executable(plot plot/plot.cpp)
target_link_libraries(plot PRIVATE ${CONAN_LIBS} shaderApi)

# Triangle plot
add_executable(trianglePlot plot/trianglePlot.cpp)
target_link_libraries(trianglePlot PRIVATE ${CONAN_LIBS} shaderApi)
