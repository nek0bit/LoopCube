#include "modelgroup.hpp"

ModelGroup::ModelGroup()
    : models{}
{

}

ModelGroup::~ModelGroup()
{

}

void ModelGroup::addModel(const std::vector<Vertex>& model)
{
    models.emplace_back(model);
}

const Model& ModelGroup::getModel(const size_t index) const
{
    return models.at(index);
}
