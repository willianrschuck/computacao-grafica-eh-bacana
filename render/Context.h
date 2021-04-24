#ifndef LEARN_OPENGL_CONTEXT_H
#define LEARN_OPENGL_CONTEXT_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Context {
public:

    Context();

    int init();

    GLFWwindow* currentWindow();

private:
    GLFWwindow* window;

};


#endif
