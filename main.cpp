#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include "render/Context.h"
#include "render/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "render/stb_image.h"
#include "render/Camera.h"

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

unsigned int createTexture(std::string path) {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureId;
}

int main() {

    context.init();

    Shader program("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    Shader lightShader("../shaders/light_vertex.glsl", "../shaders/light_fragment.glsl");
    Shader cubeShader("../shaders/cube_vertex.glsl", "../shaders/cube_fragment.glsl");


    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    float cube[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
    };

    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int cVBO, cVAO;

    glGenVertexArrays(1, &cVAO);
    glGenBuffers(1, &cVBO);

    glBindVertexArray(cVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


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


    cubeShader.use();
    cubeShader.setInt("skybox", 0);


    program.use();
    program.setInt("material.diffuseTexture", 0);
    program.setInt("material.specularTexture", 1);


    Eigen::Affine3f transform(Eigen::Translation3f(0, 0, 0));

    Eigen::Affine3f lightModel(Eigen::Translation3f(0, 1, 2));

    glfwSetInputMode(context.currentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(context.currentWindow(), mouse_callback);
    glfwSetScrollCallback(context.currentWindow(), scroll_callback);

    glEnable(GL_DEPTH_TEST);


    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(context.currentWindow())) {

        processarInputs(context.currentWindow(), &camera);



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Eigen::Vector3f camPos = camera.getPosition();
        Eigen::Vector3f direction = camera.getDirection();

        program.use();

        program.setVec3("light.position", camPos.x(), camPos.y(), camPos.z());
        program.setVec3("light.direction", direction.x(), direction.y(), direction.z());

//        program.setVec3("light.position", 0, -2, 0);
//        program.setVec3("light.direction", 0, 1, 0);
        program.setFloat("light.cutOff", 0.9263);

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


        unsigned int containterTexId = createTexture("../container2.png");
        unsigned int containterSpecularTexId = createTexture("../specular.png");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containterTexId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containterSpecularTexId);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();
        lightShader.setVec3("lightColor", 1.0f, 0.5, 1);

        transform.rotate(Eigen::AngleAxisf(0.001, Eigen::Vector3f::UnitX()));
        transform.rotate(Eigen::AngleAxisf(-0.001, Eigen::Vector3f::UnitY()));
        //transform.translate(Eigen::Vector3f (0, 1, 0));

//        unsigned int transformLoc1 = glGetUniformLocation(program.id, "model");
//        glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, lightModel.data());
//        unsigned int viewLoc1 = glGetUniformLocation(program.id, "view");
//        glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, viewMat.data());
//        unsigned int projectionLoc1 = glGetUniformLocation(program.id, "projection");
//        glUniformMatrix4fv(projectionLoc1, 1, GL_FALSE, camera.projection().data());
//
//        glBindVertexArray(cVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);



        Eigen::Matrix4f m = viewMat;
        m(0,3) = 0;
        m(1,3) = 0;
        m(2,3) = 0;
//        glDepthMask(GL_FALSE);
        cubeShader.use();
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        viewLoc = glGetUniformLocation(cubeShader.id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, m.data());
        projectionLoc = glGetUniformLocation(cubeShader.id, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, camera.projection().data());
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
//        glDepthMask(GL_TRUE);

        glfwSwapBuffers(context.currentWindow());
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}