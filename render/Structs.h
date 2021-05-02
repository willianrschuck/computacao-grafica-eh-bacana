#ifndef LEARN_OPENGL_STRUCTS_H
#define LEARN_OPENGL_STRUCTS_H

struct Vertex {
    Eigen::Vector3f Position;
    Eigen::Vector3f Normal;
    Eigen::Vector2f TexCoords;
    Eigen::Vector3f Tangent;
    Eigen::Vector3f BiTangent;
};

struct Texture {
    unsigned int id;
    std::string type;
};

#endif //LEARN_OPENGL_STRUCTS_H
