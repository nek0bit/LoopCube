#ifndef STRUCTURE_DATA_HPP
#define STRUCTURE_DATA_HPP
#include <iostream>
#include <string>

class StructureData {
public:
    StructureData(int x, int y, int id);
    ~StructureData();
    
    int x;
    int y;
    int id;
};


#endif // STRUCTURE_DATA_HPP
