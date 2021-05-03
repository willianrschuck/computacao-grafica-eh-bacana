#ifndef LEARN_OPENGL_INTERFACE_H
#define LEARN_OPENGL_INTERFACE_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Eigen/Dense>

class Interface {
public:
    static void init(GLFWwindow* window);
    static void drop();
    void render();

    Eigen::Vector3f cameraPostion;
    bool isPaused;
    float* walkSpeed;
    float* rotationSpeed;

};


#endif