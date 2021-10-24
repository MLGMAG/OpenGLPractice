add_executable(window window/window.cpp)
target_link_libraries(window ${CONAN_LIBS})

add_executable(legacyTriangle legacyTriangle/legacyTriangle.cpp)
target_link_libraries(legacyTriangle ${CONAN_LIBS})

add_executable(initGlew initGlew/initGlew.cpp)
target_link_libraries(initGlew ${CONAN_LIBS})

add_executable(glVersion glVersion/glVersion.cpp)
target_link_libraries(glVersion ${CONAN_LIBS})

include_directories(api/shaderApi)
add_library(
        shaderApi STATIC
        api/shaderApi/shaderApi.h
        api/shaderApi/shaderApi.cpp
)

add_executable(triangle triangle/triangle.cpp)
target_link_libraries(triangle PRIVATE ${CONAN_LIBS} shaderApi)

add_executable(square square/square.cpp)
target_link_libraries(square PRIVATE ${CONAN_LIBS} shaderApi)

add_subdirectory(catInOrange)

# Vertex Array Object Practice
add_executable(vertexArrayObjectPractice vertexArrayObjectPractice/vertexArrayObjectPractice.cpp)
target_link_libraries(vertexArrayObjectPractice PRIVATE ${CONAN_LIBS} shaderApi)
