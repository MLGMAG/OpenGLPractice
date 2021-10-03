add_executable(window window/window.cpp)
target_link_libraries(window ${CONAN_LIBS})

add_executable(legacyTriangle legacyTriangle/legacyTriangle.cpp)
target_link_libraries(legacyTriangle ${CONAN_LIBS})

add_executable(initGlew initGlew/initGlew.cpp)
target_link_libraries(initGlew ${CONAN_LIBS})

add_executable(glVersion glVersion/glVersion.cpp)
target_link_libraries(glVersion ${CONAN_LIBS})

add_executable(triangle triangle/triangle.cpp)
target_link_libraries(triangle ${CONAN_LIBS})
