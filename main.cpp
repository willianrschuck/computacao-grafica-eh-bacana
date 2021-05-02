#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include "render/Context.h"
#include "render/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "render/stb_image.h"
#include "render/Camera.h"
#include "render/BasicObjLoader.h"
#include "render/Model.h"

#include <vector>

Context context;
Camera camera({0, 0, 3});

void processarInputs(GLFWwindow* window, Camera* camera) {

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

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.rotate(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0) {
        camera.zoomIn();
    } else if (yoffset < 0) {
        camera.zoomOut();
    }
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
        if (data)
        {
            std::cout << faces[i].c_str() << std::endl;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
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

Texture createTexture(std::string type, std::string path) {
    Texture texture;
    texture.type = type;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

Mesh createMesh(std::string name, std::string textura, std::string specularTex = "", std::string normalTex = "") {

    static Texture normal = createTexture("material.normalMap", "../texturas/normal.png");

    BasicObjLoader obj;
    obj.ler("../objetos/"+name+".obj");
    std::vector<Texture> tex;
    tex.push_back(createTexture("material.diffuseTexture", "../texturas/"+textura));
    if (!specularTex.empty()) tex.push_back(createTexture("material.specularTexture", "../texturas/"+specularTex));
    if (!normalTex.empty()) {
        tex.push_back(createTexture("material.normalMap", "../texturas/"+normalTex));
    } else {
        tex.push_back(normal);
    }
    Mesh mesh(obj.vertices, obj.indices, tex);
    return mesh;

}

int main() {

    context.init();

    Shader program("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    Shader lightShader("../shaders/light_vertex.glsl", "../shaders/light_fragment.glsl");
    Shader cubeShader("../shaders/cube_vertex.glsl", "../shaders/cube_fragment.glsl");

    BasicObjLoader obj;
    obj.ler("../objetos/caxa.obj");
    std::vector<std::string> faces
            {
                    "../cube/right.jpg",
                    "../cube/left.jpg",
                    "../cube/top.jpg",
                    "../cube/bottom.jpg",
                    "../cube/front.jpg",
                    "../cube/back.jpg"
            };
    unsigned int cubemapTexture = loadCubemap(faces);
    std::vector<Texture> sky;
    Texture skyTex;
    skyTex.type = "skybox";
    skyTex.id = cubemapTexture;
    Mesh skybox(obj.vertices, obj.indices, sky);

    Model casa;

    casa.addMesh(createMesh("casa/base", "stone-floor/color.jpg", "stone-floor/specular.jpg", "stone-floor/normal.jpg"));
    casa.addMesh(createMesh("casa/chao-lajota", "tiles/color.jpg","tiles/specular.jpg", "tiles/normal.jpg"));
    casa.addMesh(createMesh("casa/chao-parque", "chao/color.jpg","chao/specular.jpg", "chao/normal.jpg"));
    casa.addMesh(createMesh("casa/fundacao", "brick/color.png", "brick/specular.png", "brick/normal.png"));
    casa.addMesh(createMesh("casa/gramado", "grass/color.jpg", "grass/specular.jpg", "grass/normal.jpg"));
    casa.addMesh(createMesh("casa/parede-exterior", "wall/color.jpg", "wall/specular.jpg", "wall/normal.jpg"));
    casa.addMesh(createMesh("casa/parede-interior", "interior.png"));
    casa.addMesh(createMesh("casa/parede-lajota", "lajota.png"));
    casa.addMesh(createMesh("casa/teto", "telha/cor.jpg", "telha/specular.jpg", "telha/normal.jpg"));

    Mesh xadrez = createMesh("brush", "grimstroke_weapon_color.tga", "grimstroke_weapon_specularMask.tga", "grimstroke_weapon_normal.tga");

    glfwSetInputMode(context.currentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(context.currentWindow(), mouse_callback);
    glfwSetScrollCallback(context.currentWindow(), scroll_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MIPMAP);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(context.currentWindow())) {

        processarInputs(context.currentWindow(), &camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Eigen::Vector3f camPos = camera.getPosition();
        Eigen::Vector3f direction = camera.getDirection();

        program.use();

        Eigen::Vector3f soma = camPos.cross(Eigen::Vector3f(0,1,0));

        //camPos = camPos - soma * 0.2;

        program.setVec3("light.position", camPos.x(), camPos.y(), camPos.z());
        program.setVec3("light.direction", direction.x(), direction.y(), direction.z());


//        program.setVec3("light.position", 0, -2, 0);
//        program.setVec3("light.direction", 0, 1, 0);
        program.setFloat("light.cutOff", 0.9263);
        program.setFloat("light.outerCutOff", 0.9563);

        program.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        program.setVec3("lightColor",  1.0f, 0.5f, 1.0f);
        program.setVec3("viewPos", camPos.x(), camPos.y(), camPos.z());

        program.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        program.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        program.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        program.setFloat("material.shininess", 32.0f);

//        program.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        program.setVec3("light.ambient",  0.15f, 0.1f, 0.1f);
        program.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        program.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        program.setFloat("light.constant",  1.0f);
        program.setFloat("light.linear",    0.09f);
        program.setFloat("light.quadratic", 0.032f);


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

        lightShader.use();
        lightShader.setVec3("lightColor", 1.0f, 0.5, 1);

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

        glfwSwapBuffers(context.currentWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}