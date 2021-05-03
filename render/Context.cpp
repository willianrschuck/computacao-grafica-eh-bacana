#include "Context.h"

Context::Context() = default;



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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MIPMAP);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    glfwSwapInterval(0);

    return 0;

}

GLFWwindow *Context::currentWindow() {
    return window;
}

void Context::setCamera(Camera *cam) {
    camera = cam;
}

void Context::processarInputs() {

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {

        static double lastTimePaused = -1;
        double deltaTimeSincePaused = glfwGetTime() - lastTimePaused;

        if (deltaTimeSincePaused < 0.5) return; // Só pode pausar/despausar a casa 500 milisegundos

        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        lastTimePaused = glfwGetTime();

    }

    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->moveFoward();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->moveBack();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->moveLeft();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->moveRight();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->moveUp();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->moveDown();
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

}

Camera *Context::getCamera() {
    return camera;
}

void Context::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        camera->rotate(xpos, ypos);
    }
}

void Context::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!mouseEnabled) {
        if (yoffset > 0) {
            camera->zoomIn();
        } else if (yoffset < 0) {
            camera->zoomOut();
        }
    }
}

