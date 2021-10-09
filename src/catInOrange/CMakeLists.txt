add_executable(catInOrange catInOrange.cpp)
target_link_libraries(catInOrange PRIVATE ${CONAN_LIBS} shaderApi)
