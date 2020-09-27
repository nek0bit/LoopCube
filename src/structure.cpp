#include "structure.hpp"

Structure::Structure(int x, int y) : data{}, x{x}, y{y} {}

Structure::~Structure() {}

std::vector<StructureData>& Structure::get_data() {
    return data;
}

int Structure::get_x() {
    return x;
}

int Structure::get_y() {
    return y;
}