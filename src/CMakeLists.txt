include_directories(api/shaderApi)
add_library(
        shaderApi STATIC
        api/shaderApi/shaderApi.h
        api/shaderApi/shaderApi.cpp
)

# Textures Api
include_directories(api/texturesApi)
add_library(
        texturesApi STATIC
        api/texturesApi/texturesApi.h
        api/texturesApi/texturesApi.cpp
)
add_executable(texturesApiTest api/texturesApi/texturesApi.cpp api/texturesApi/texturesApi.h)
target_link_libraries(texturesApiTest PRIVATE ${CONAN_LIBS})

# Camera Api
include_directories(api/cameraApi)
add_library(
        cameraApi STATIC
        api/cameraApi/cameraApi.h
        api/cameraApi/cameraApi.cpp
)
add_executable(cameraApiTest api/cameraApi/cameraApi.cpp api/cameraApi/cameraApi.h api/cameraApi/cameraApiTest.cpp)
target_link_libraries(cameraApiTest PRIVATE ${CONAN_LIBS} shaderApi texturesApi)

add_subdirectory(2d)

add_subdirectory(3d)
