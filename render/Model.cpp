#include "Model.h"

void Model::Draw(Shader &shader) {
    for (auto mesh : meshes) {
        mesh.Draw(shader);
    }
}

void Model::addMesh(Mesh mesh) {
    meshes.push_back(mesh);
}
