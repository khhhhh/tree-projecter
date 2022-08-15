#include "tree.h"


using json = nlohmann::json;
Tree::Tree() {
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
    propsCopy = {
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
    init();
}

Tree::Tree(json j)
{
    props.update(j);
    propsCopy.update(j);
    init();
}

Tree::~Tree()
{
    delete meshTree;
    delete meshTwig;
}

void Tree::generate() {
    Proctree::Properties props(this->props);
    Mesh::changeTree(*meshTree, *meshTwig, props);
}

void Tree::generate(json Props) {
    Proctree::Properties props(Props);
    Mesh::changeTree(*meshTree, *meshTwig, props);
}

void Tree::grow(int procent)
{
    changeGrowth(procent);
    props.update(propsCopy);
}


vec3 Tree::getPos() {
    return meshTree->pos;
}
void Tree::setPos(vec3 pos) {
    meshTree->pos = pos;
    meshTwig->pos = pos;
}

void Tree::setProps(json j)
{
    this->props.update(j);
    this->propsCopy.update(j);
}

json Tree::getProps()
{
    return props;
}


void Tree::changeGrowth(int procent)
{
    int growSteps = 100;
    float procentF = (float)procent/ growSteps;
    /*
    if(procent < 10)
    {
        if(procent == 0)
            procent = 1;
        procentF = 0.1 - ((float)procent / 500.0f);
    }
    */

    int seasonLvl = 0;
    float mInitialBranchLength = props["mInitialBranchLength"];
    float mMaxRadius = props["mMaxRadius"];
    float mTrunkLength = props["mTrunkLength"];
    float mTwigScale = props["mTwigScale"];
    float mDropAmount = props["mDropAmount"];
    float mGrowAmount = props["mGrowAmount"];
    float mClimbRate = props["mClimbRate"];
    float mLengthFalloffFactor = props["mLengthFalloffFactor"];
    float mTrunkKink = props["mTrunkKink"];

    mInitialBranchLength *= procentF;
    mMaxRadius *= procentF;
    mTrunkLength *=procentF;
    mDropAmount *= procentF;
    mGrowAmount *= procentF;
    seasonLvl = procent;
    mClimbRate *= procentF;
    mLengthFalloffFactor *= procentF;
    mTrunkKink *= procentF;

    if(procent >= 50)
        mTwigScale *= (procentF - 0.5) * 2;
    else
        mTwigScale = 0;

    props["mInitialBranchLength"] = mInitialBranchLength;
    props["mMaxRadius"] = mMaxRadius;
    props["mTrunkLength"] = mTrunkLength;
    props["mTwigScale"] = mTwigScale;
    props["mDropAmount"] = mDropAmount;
    props["mGrowAmount"] = mGrowAmount;
    props["mClimbRate"] = mClimbRate;
    props["mLengthFalloffFactor"] = mLengthFalloffFactor;
    props["mSeason"] = seasonLvl;
    props["mTrunkKink"] = mTrunkKink;

    generate();
}

void Tree::init()
{
    meshTwig = new Mesh(GL_TRIANGLES);
    meshTree = new Mesh(GL_TRIANGLES);

    this->generate();
    meshTree->pos = {0, 0, 0};
    meshTwig->pos = {0, 0, 0};

    texture.tree = 0;
    texture.twig = 1;
    //propsCopy.update(props);
}

Mesh *Tree::getMeshTwig() const
{
    return meshTwig;
}

void Tree::setMeshTwig(Mesh *newMeshTwig)
{
    meshTwig = newMeshTwig;
}

Mesh *Tree::getMeshTree() const
{
    return meshTree;
}

void Tree::setMeshTree(Mesh *newMeshTree)
{
    meshTree = newMeshTree;
}
