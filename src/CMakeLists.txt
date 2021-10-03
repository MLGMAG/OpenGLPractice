add_executable(window window/window.cpp)
target_link_libraries(window ${CONAN_LIBS})

add_executable(triangle triangle/triangle.cpp)
target_link_libraries(triangle ${CONAN_LIBS})
