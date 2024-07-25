cmake_minimum_required(VERSION 3.18.1)

project(LearnOpenGL VERSION 0.0.1)

# 使用 C++ 17 标准
set(CMAKE_CXX_STANDARD 17)

# 设置代码文件目录
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# 添加头文件
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
include_directories(${HEADER_DIR}/ ${LIB_DIR}/)

set(GLFW_LINK ${LIB_DIR}/libglfw.3.dylib)
set(ASSIMP_LINK_0 ${LIB_DIR}/libassimp.3.1.1.dylib)
set(ASSIMP_LINK_1 ${LIB_DIR}/libassimp.3.dylib)
set(ASSIMP_LINK_2 ${LIB_DIR}/libassimp.dylib)

set(SOURCES ${SRC_DIR}/glad.c ${SRC_DIR}/main.cpp
        include/Cube.h
        include/Quad.h
        include/Plane.h
        include/Sphere.h
        include/Skybox.h
        include/Utils.h
        include/Window.h
        include/Transform.h
        include/Light.h
        include/Material.h
        include/IBL.h
)
add_executable(LearnOpenGL ${SOURCES})
# 链接GLFW库

target_link_libraries(LearnOpenGL ${GLFW_LINK})
target_link_libraries(LearnOpenGL ${ASSIMP_LINK_0})
target_link_libraries(LearnOpenGL ${ASSIMP_LINK_2})
target_link_libraries(LearnOpenGL ${ASSIMP_LINK_1})
# 链接系统的 OpenGL 框架和其他必要的库
if (APPLE)
    target_link_libraries(LearnOpenGL "-framework OpenGL")
    target_link_libraries(LearnOpenGL "-framework Cocoa")
endif()
