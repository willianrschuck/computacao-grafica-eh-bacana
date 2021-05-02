cmake_minimum_required(VERSION 3.17)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIR "/home/willian/.include")
include_directories(${INCLUDE_DIR})

find_package(GLEW 2.0 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(learn_opengl main.cpp render/Shader.cpp render/Shader.h render/Context.cpp render/Context.h render/stb_image.h render/Camera.cpp render/Camera.h render/Mesh.cpp render/Mesh.h render/Model.cpp render/Model.h render/BasicObjLoader.h render/BasicObjLoader.cpp render/Structs.h)

target_link_libraries(learn_opengl GLEW::GLEW)
target_link_libraries(learn_opengl OpenGL::GL)
target_link_libraries(learn_opengl glfw)