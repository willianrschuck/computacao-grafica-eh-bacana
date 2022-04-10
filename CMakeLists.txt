cmake_minimum_required(VERSION 3.13)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIR "/home/willian/.include")
set(IMGUI_PATH "/home/willian/.include/imgui")

# set(GLFW_PATH "include/glfw/src")

# add_subdirectory(${GLFW_PATH})

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library("ImGui" STATIC ${IMGUI_SOURCES})

include_directories(${INCLUDE_DIR})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})

find_package(GLEW 2.0 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(learn_opengl main.cpp render/Shader.cpp render/Shader.h render/Context.cpp render/Context.h render/stb_image.h render/Camera.cpp render/Camera.h render/Mesh.cpp render/Mesh.h render/Model.cpp render/Model.h render/BasicObjLoader.h render/BasicObjLoader.cpp render/Structs.h render/Interface.cpp render/Interface.h)
target_link_libraries(learn_opengl GLEW::GLEW OpenGL::GL glfw)



target_link_libraries(learn_opengl glfw)

target_link_libraries(learn_opengl ImGui)
