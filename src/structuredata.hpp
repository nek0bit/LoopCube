#ifndef STRUCTURE_DATA_HPP
#define STRUCTURE_DATA_HPP
#include <iostream>
#include <string>

class StructureData {
public:
    StructureData(int x, int y, std::string id);
    ~StructureData();
    
    int x;
    int y;
    std::string id;
};


#endif // STRUCTURE_DATA_HPP