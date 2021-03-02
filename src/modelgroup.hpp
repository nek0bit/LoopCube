#pragma once
#include <vector>
#include "model.hpp"
#include "vertex.hpp"

enum models_e
{
    MODEL_SQUARE
};

struct ModelGroup
{
    ModelGroup();
    ~ModelGroup();
    
    void addModel(const std::vector<Vertex>& model);
    const Model& getModel(const size_t index);
private:
    std::vector<Model> models;
};
