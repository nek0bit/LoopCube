#include "modelgroup.hpp"

ModelGroup::ModelGroup()
    : models{}
{

}

ModelGroup::~ModelGroup()
{

}

void ModelGroup::addModel(const Model& model)
{
    models.emplace_back(model);
}

const Model& ModelGroup::getModel(const size_t index)
{
    return models.at(index);
}
