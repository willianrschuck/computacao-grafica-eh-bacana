#include "Context.h"

Context::Context() = default;

void resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int Context::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Falha ao criar a janela." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Falha ao iniciar o GLEW." << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetWindowSizeCallback(window, resize_callback);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    return 0;

}

GLFWwindow *Context::currentWindow() {
    return window;
}

void Context::setCamera(Camera *cam) {

}
