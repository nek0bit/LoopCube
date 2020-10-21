#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP
#include <iostream>
#include <vector>
#include "structuredata.hpp"

class Structure {
public:
	Structure(int x, int y);
	virtual ~Structure();
	
	virtual std::vector<StructureData>& get_data();
	virtual int get_x();
	virtual int get_y();
protected:
	std::vector<StructureData> data;
	int x;
	int y;	  
};

#endif // STRUCTURE_HPP
