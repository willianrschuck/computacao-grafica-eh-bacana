#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include "render/Context.h"
#include "render/Shader.h"
#include "render/stb_image.h"
#include "render/Camera.h"
#include "render/BasicObjLoader.h"
#include "render/Model.h"
#include "render/Interface.h"

#include <vector>

Context context;

void resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    context.getCamera()->newViewAspectRatio(width/(float) height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    context.mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    context.scroll_callback(window, xoffset, yoffset);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

        } else {

            std::cout << "Falha ao carregar cubemap. Arquivo: " << faces[i] << std::endl;
            stbi_image_free(data);

        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int main() {

    context.init();

    Interface::init(context.currentWindow());
    Interface interface;

    int width, height;
    glfwGetWindowSize(context.currentWindow(), &width, &height);

    Camera camera({-0.5, 2, -6}, width/(float)height);
    context.setCamera(&camera);
    interface.walkSpeed = &camera.walkSpeed;
    interface.rotationSpeed = &camera.rotationSpeed;

    glfwSetCursorPosCallback(context.currentWindow(), mouse_callback);
    glfwSetScrollCallback(context.currentWindow(), scroll_callback);
    glfwSetWindowSizeCallback(context.currentWindow(), resize_callback);

    Shader program("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    Shader cubeShader("../shaders/cube_vertex.glsl", "../shaders/cube_fragment.glsl");

    BasicObjLoader obj;
    obj.ler("../objetos/caxa.obj");
    std::vector<std::string> faces {
        "../cube/right.jpg", "../cube/left.jpg",
        "../cube/top.jpg", "../cube/bottom.jpg",
        "../cube/front.jpg", "../cube/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    std::vector<Texture> sky;
    Texture skyTex;
    skyTex.type = "skybox";
    skyTex.id = cubemapTexture;
    Mesh skybox(obj.vertices, obj.indices, sky);

    Model casa;
    {
        casa.addMesh(
                Mesh::createMesh("casa/base", "terracota/color.jpg", "terracota/specular.jpg", "terracota/normal.jpg"));
        casa.addMesh(Mesh::createMesh("casa/chao-lajota", "tiles/color.jpg", "tiles/specular.png", "tiles/normal.jpg"));
        casa.addMesh(Mesh::createMesh("casa/chao-parque", "chao/color.jpg", "chao/specular.jpg", "chao/normal.jpg"));
        casa.addMesh(Mesh::createMesh("casa/fundacao", "brick/color.png", "brick/specular.png", "brick/normal.png"));
        casa.addMesh(Mesh::createMesh("casa/gramado", "grass/color.jpg", "grass/specular.jpg", "grass/normal.jpg"));
        casa.addMesh(
                Mesh::createMesh("casa/parede-exterior", "wall/color.jpg", "wall/specular.jpg", "wall/normal.jpg"));
        casa.addMesh(Mesh::createMesh("casa/parede-interior", "interior.png"));
        casa.addMesh(Mesh::createMesh("casa/parede-lajota", "lajota.png"));
        casa.addMesh(Mesh::createMesh("casa/teto", "telha/cor.jpg", "telha/specular.png", "telha/normal.jpg"));
    }

    Model brush;
    brush.addMesh(Mesh::createMesh("brush", "grimstroke-weapon/color.tga", "grimstroke-weapon/specular.tga", "grimstroke-weapon/normal.tga"));

    Model queijo;
    queijo.addMesh(Mesh::createMesh("queijo", "queijo.png"));

    Model xadrez;
    xadrez.addMesh(Mesh::createMesh("xadrez", "xadrez.png"));

    Model armario;
    armario.addMesh(Mesh::createMesh("armario", "armario.png"));

    Model bule;
    bule.addMesh(Mesh::createMesh("bule", "bule.png"));

    Model copo;
    copo.addMesh(Mesh::createMesh("copo", "copo.png"));

    Model carro1;
    carro1.addMesh(Mesh::createMesh("carro1", "carro1.png"));

    Model carro2;
    carro2.addMesh(Mesh::createMesh("carro2", "carro2.png"));

    /* Inicialização dos valores do Shader */
    {
        program.use();

        program.setFloat("light.cutOff", 0.9263);
        program.setFloat("light.outerCutOff", 0.9563);

        program.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        program.setVec3("lightColor",  1.0f, 0.5f, 1.0f);

        program.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        program.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        program.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        program.setFloat("material.shininess", 32.0f);

        program.setVec3("light.ambient",  0.15f, 0.1f, 0.1f);
        program.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);
        program.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        program.setFloat("light.constant",  1.0f);
        program.setFloat("light.linear",    0.09f);
        program.setFloat("light.quadratic", 0.032f);
    }

    while (!glfwWindowShouldClose(context.currentWindow())) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        interface.isPaused = glfwGetInputMode(context.currentWindow(), GLFW_CURSOR) != GLFW_CURSOR_DISABLED;

        context.processarInputs();

        interface.cameraPostion = context.getCamera()->getPosition();

        Eigen::Vector3f direction = camera.getDirection();
        Eigen::Vector3f camPos = camera.getPosition();

        program.use();
        program.setVec3("viewPos", camPos.x(), camPos.y(), camPos.z());
        program.setVec3("light.position", camPos.x(), camPos.y(), camPos.z());
        program.setVec3("light.direction", direction.x(), direction.y(), direction.z());

        Eigen::Matrix4f viewMat = camera.view();

        unsigned int transformLoc;
        unsigned int viewLoc;
        unsigned int projectionLoc;

        Eigen::Matrix4f i = Eigen::Matrix4f::Identity();
        transformLoc = glGetUniformLocation(program.id, "model");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, i.data());
        viewLoc = glGetUniformLocation(program.id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.data());
        projectionLoc = glGetUniformLocation(program.id, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera.projection().data());

        casa.Draw(program);

        brush.Draw(program);
        queijo.Draw(program);
        xadrez.Draw(program);
        armario.Draw(program);
        bule.Draw(program);
        copo.Draw(program);
        carro1.Draw(program);
        carro2.Draw(program);

        Eigen::Matrix4f m = viewMat;
        m(0,3) = 0;
        m(1,3) = 0;
        m(2,3) = 0;

        cubeShader.use();

        glDepthFunc(GL_LEQUAL);
        viewLoc = glGetUniformLocation(cubeShader.id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, m.data());
        projectionLoc = glGetUniformLocation(cubeShader.id, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera.projection().data());

        skybox.Draw(cubeShader);

        interface.render();

        glfwSwapBuffers(context.currentWindow());
        glfwPollEvents();

    }

    Interface::drop();

    glfwTerminate();
    return 0;
}