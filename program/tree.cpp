#include "tree.h"


using json = nlohmann::json;
Tree::Tree() {
    meshTwig = new Mesh(GL_TRIANGLES);
    meshTree = Mesh::generateTree(meshTwig);
    meshTree->pos = {0, 0, 0};
    meshTwig->pos = {0, 0, 0};

    texture.tree = 0;
    texture.twig = 1;
    /*
    props = {
        {"mSeed" , 262},
        {"mSegments" , 6},
        {"mLevels" , 5},
        {"mVMultiplier" , 0.36f},
        {"mTwigScale" , 0.39f},
        {"mInitialBranchLength" , 0.49f},
        {"mLengthFalloffFactor" , 0.85f},
        {"mLengthFalloffPower" , 0.99f},
        {"mClumpMax" , 0.454f},
        {"mClumpMin" , 0.404f},
        {"mBranchFactor" , 2.45f},
        {"mDropAmount" , -0.1f},
        {"mGrowAmount" , 0.235f},
        {"mSweepAmount" , 0.01f},
        {"mMaxRadius" , 0.139f},
        {"mClimbRate" , 0.371f},
        {"mTrunkKink" , 0.093f},
        {"mTreeSteps" , 5},
        {"mTaperRate" , 0.947f},
        {"mRadiusFalloffRate" , 0.73f},
        {"mTwistRate" , 3.02f},
        {"mTrunkLength" , 2.4f}
    };
    */
}

void Tree::generate() {
    Proctree::Properties props(this->props);
    Mesh::changeTree(*meshTree, *meshTwig, props);
}

vec3 Tree::getPos() {
    return meshTree->pos;
}
void Tree::setPos(vec3 pos) {
    meshTree->pos = pos;
    meshTwig->pos = pos;
}

void Tree::setProps(json props)
{
    this->props = props;
}

json Tree::getProps()
{
    return props;
}
