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

add_subdirectory(2d)
