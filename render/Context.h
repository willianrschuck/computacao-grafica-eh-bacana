#ifndef LEARN_OPENGL_CONTEXT_H
#define LEARN_OPENGL_CONTEXT_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class Context {
public:

    Context();
    int init();
    GLFWwindow* currentWindow();

    void processarInputs();
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    void setCamera(Camera* cam);
    Camera* getCamera();

private:

    bool mouseEnabled;
    GLFWwindow* window{};
    Camera* camera{};

};


#endif
