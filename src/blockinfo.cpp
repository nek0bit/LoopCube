#include "blockinfo.hpp"

BlockInfo::BlockInfo(std::string id, std::string name, int texture_id, bool no_collision) {
    this->id = id;
    this->name = name;
    this->texture_id = texture_id;
    this->no_collision = no_collision;
}

BlockInfo::~BlockInfo() {

}

std::string BlockInfo::get_id() const{
    return id;
}

std::string BlockInfo::get_name() const{
    return name;
}

int BlockInfo::get_texture_id() const{
    return texture_id;
}

bool BlockInfo::get_no_collision() const{
    return no_collision;
}
