#ifndef LEARN_OPENGL_MESH_H
#define LEARN_OPENGL_MESH_H

#include "Shader.h"
#include <Eigen/Dense>
#include <vector>
#include "Structs.h"

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    static Mesh createMesh(std::string name, std::string textura, std::string specularTex = "", std::string normalTex = "");
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int>, std::vector<Texture> textures);

    void Draw(Shader &shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};


#endif //LEARN_OPENGL_MESH_H
