#ifndef OBJ_H_INCLUDED
#define OBJ_H_INCLUDED

#include <string>
#include <vector>
#include <Eigen/Dense>


#include "Mesh.h"

class BasicObjLoader
{
public:
    BasicObjLoader();
    std::vector<Eigen::Vector3f> vert;
    std::vector<Eigen::Vector2f> text;
    std::vector<Eigen::Vector3f> norm;


    std::vector<Eigen::Vector3f> vecV;
    std::vector<Eigen::Vector2f> vecT;
    std::vector<Eigen::Vector3f> vecN;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool ler(std::string caminho);
private:


    static std::vector<std::string> splitstring(std::string s, char delimitador);

    Vertex processVertex(std::string basicString);

    Vertex createVertex(std::string basicString);
};

#endif