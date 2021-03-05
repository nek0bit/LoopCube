#include "modelgroup.hpp"

ModelGroup::ModelGroup()
    : models{}
{}

ModelGroup::~ModelGroup()
{}

void ModelGroup::addModel(const GLuint shader, const std::vector<Vertex>& model)
{
    models.emplace_back(shader, model);
}

const Model& ModelGroup::getModel(const size_t index) const
{
    return models.at(index);
}
