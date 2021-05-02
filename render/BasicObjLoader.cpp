#include "BasicObjLoader.h"
using namespace std;
using namespace Eigen;

#include <iostream>
#include <fstream>
#include <algorithm>

BasicObjLoader::BasicObjLoader()= default;

bool BasicObjLoader::ler(string caminho)
{
    streambuf *cinbuf = std::cin.rdbuf(); //salvar o buffer atual!

    ifstream file(caminho);
    if (file.is_open())
		cin.rdbuf(file.rdbuf()); //redirecionar std::cin para o arquivo!
	else
        return false;

    vector<string> vecF;
    string codigo;

    long count = 0;

    while(cin >> codigo){

        if (!std::cin)
            break;

        std::transform(codigo.begin(), codigo.end(), codigo.begin(), [](unsigned char c){ return std::tolower(c); });

        if (codigo.size() > 0 && codigo[0] == '#') {
            string aux; //jogar fora
            getline(cin,aux);
		}
        else if( codigo == "v"){
            Vector3f v;
            cin >> v[0];
            cin >> v[1];
            cin >> v[2];

            vecV.push_back(v);
        }
        else if( codigo == "vt"){
            Vector2f vt;
            cin >> vt[0];
            cin >> vt[1];

            vecT.push_back(vt);
        }
        else if( codigo == "vn"){
            Vector3f vn;
            cin >> vn[0];
            cin >> vn[1];
            cin >> vn[2];

            vecN.push_back(vn);
        }
        else if(codigo == "f"){
            string f;

            cin >> f;
            Vertex v1 = createVertex(f);
            cin >> f;
            Vertex v2 = createVertex(f);
            cin >> f;
            Vertex v3 = createVertex(f);

            Eigen::Vector3f edge1 = v2.Position - v1.Position;
            Eigen::Vector3f edge2 = v3.Position - v1.Position;
            Eigen::Vector2f deltaUV1 = v2.TexCoords - v1.TexCoords;
            Eigen::Vector2f deltaUV2 = v3.TexCoords - v1.TexCoords;

            float theF = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

            Eigen::Vector3f tangent;
            Eigen::Vector3f bitangent;

            tangent[0] = theF * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x());
            tangent[1] = theF * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y());
            tangent[2] = theF * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z());

            bitangent[0] = theF * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x());
            bitangent[1] = theF * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y());
            bitangent[2] = theF * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z());

            v1.Tangent = tangent;
            v2.Tangent = tangent;
            v3.Tangent = tangent;

            v1.BiTangent = bitangent;
            v2.BiTangent = bitangent;
            v3.BiTangent = bitangent;

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            indices.push_back(count++);
            indices.push_back(count++);
            indices.push_back(count++);

        }
    }

    //terminou a leitura!
	file.close(); //fechar o arquivo!
	std::cin.rdbuf(cinbuf);   //restabelecer a entrada padrão novamente!


    vert.clear();
    text.clear();
    norm.clear();



    //verifica erros na leitura dos dados
    if( (vert.size()< 0) || (text.size()>0 && text.size() != vert.size()) || (norm.size()>0 && norm.size() != vert.size()) )
    {
        cout << "Erro OBJ: Erro ao compilar as faces! O numero de coordenadas de vertices deve ser igual ao numero de coordendas de textura e de normais!\n";
        vert.clear();
        text.clear();
        norm.clear();
        return false;
    }

	return true;
}

std::vector<std::string> BasicObjLoader::splitstring(std::string s, char delimitador)
{
    std::vector<std::string> pedacos;
    std::string pedaco;
    std::istringstream strstream(s);
    while (std::getline(strstream, pedaco, delimitador))
    {
        pedacos.push_back(pedaco);
    }

    return pedacos;
}

Vertex BasicObjLoader::createVertex(std::string faceParameters) {
    vector<string> d = BasicObjLoader::splitstring(faceParameters, '/');
    int nDados = d.size();

    int vIdx =-1, tIdx =-1, nIdx =-1;

    if (nDados > 0 && !d[0].empty()) vIdx = std::stoi(d[0]);
    if (nDados > 1 && !d[1].empty()) tIdx = std::stoi(d[1]);
    if (nDados > 2 && !d[2].empty()) nIdx = std::stoi(d[2]);

    vIdx--;
    tIdx--;
    nIdx--;

    Vertex v;

    if (vIdx >= 0 && vIdx < vecV.size()) {
        v.Position = vecV[vIdx];
    }
    if (tIdx >= 0 && tIdx < vecT.size()) {
        v.TexCoords = vecT[tIdx];
    }
    if (nIdx >= 0 && nIdx < vecN.size()) {
        v.Normal = vecN[nIdx];
    }

    return v;
}
