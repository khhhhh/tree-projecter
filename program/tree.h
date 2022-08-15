#ifndef TREE_H
#define TREE_H

#include "mesh.h"

struct textureIndex {
    int tree;
    int twig;
};

class Tree
{
    void changeGrowth(int procent);
    json props;
    json propsCopy;
    Mesh *meshTree, *meshTwig;

    void init();
public:

    textureIndex texture;
    Tree();
    Tree(json j);
    ~Tree();

    void generate();
    void generate(json props);
    void grow(int procent);
    vec3 getPos();
    void setPos(vec3 pos);
    void setProps(json props);
    json getProps();
    Mesh *getMeshTree() const;
    void setMeshTree(Mesh *newMeshTree);
    Mesh *getMeshTwig() const;
    void setMeshTwig(Mesh *newMeshTwig);
};

#endif // TREE_H
