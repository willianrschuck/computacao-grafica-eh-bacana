//
// Created by willian on 27/04/2021.
//

#ifndef LEARN_OPENGL_MODEL_H
#define LEARN_OPENGL_MODEL_H

#include <vector>
#include "Shader.h"
#include "Mesh.h"

class Model {
    public:
        void Draw(Shader &shader);
        void addMesh(Mesh mesh);
    private:
        std::vector<Mesh> meshes;
};


#endif //LEARN_OPENGL_MODEL_H
