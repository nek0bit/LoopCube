#pragma once
#include <vector>
#include "model.hpp"
#include "vertex.hpp"

enum models_e
{
    MODEL_SQUARE,
    MODEL_PLAYER
};

struct ModelGroup
{
    ModelGroup();
    ~ModelGroup();
    
    void addModel(const GLuint shader, const std::vector<Vertex>& model);
    const Model& getModel(const size_t index) const;
    std::vector<Model> models;
};
