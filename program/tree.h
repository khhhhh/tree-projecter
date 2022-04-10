#ifndef TREE_H
#define TREE_H

#include "mesh.h"

struct textureIndex {
    int tree;
    int twig;
};

class Tree
{
public:

    Mesh *meshTree, *meshTwig;
    json props;
    textureIndex texture;
    Tree();

    void generate();
    vec3 getPos();
    void setPos(vec3 pos);
    void setProps(json props);
    json getProps();
};

#endif // TREE_H
