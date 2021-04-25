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
    void setCamera(Camera* cam);

private:
    GLFWwindow* window{};
    Camera* camera{};

};


#endif
