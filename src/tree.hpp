#ifndef TREE_HPP
#define TREE_HPP
#include <iostream>
#include "constants.hpp"
#include "structure.hpp"

class Tree: public Structure {
public:
    Tree(int x, int y);
    ~Tree();
};

#endif // TREE_HPP
