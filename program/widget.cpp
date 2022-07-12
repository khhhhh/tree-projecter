#include "widget.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFile>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <string>

using json = nlohmann::json;

OpenGlWidget::~OpenGlWidget()
{
}

void OpenGlWidget::timerStop()
{
    timer.stop();
}

void OpenGlWidget::timerStart()
{
    timer.start();
}

void OpenGlWidget::updateOpenGl()
{
    update();
}

void OpenGlWidget::growTree(int index, int procent)
{
    makeCurrent();
    trees->at(index)->growTree(procent);
}

void OpenGlWidget::initializeGL() {
    de = 0;
    angle = 0.0f;
    ax=ay=dax=day=0;

    lightPosition = {10, 5, 0};

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    camera = new Camera();

    //textures
    Texture *treeTex, *twigTex;
    Texture *buildingTex = new Texture();

    treeTex = new Texture();
    twigTex = new Texture();

    treeTex->loadFromImage(":/textures/trees/1.jpg");
    twigTex->loadFromImage(":/textures/twigs/twig1.png");
    buildingTex->loadFromImage(":/textures/building/building.jpg");

    textures.push_back(treeTex);
    textures.push_back(twigTex);
    textures.push_back(buildingTex);


    //first Tree
    Tree *firstTree = new Tree();
    firstTree->texture.tree = 0;
    firstTree->texture.twig = 1;
    trees->push_back(firstTree);
    listWidget->addItem("Tree 1");
    listWidget->setCurrentRow(0);

    // terrain and walls
    building = Mesh::createBuilding(buildingTex->width(), buildingTex->height(), 100.0f);
    building->pos = {-50, 0, -50};

    terrain = Mesh::createTerrain();
    terrain->pos = {-50, 0, -50};


    phongProgram = new GLSLProgram();
    phongProgram->compileShaderFromFile(":/shaders/phong.vert", GL_VERTEX_SHADER);
    phongProgram->compileShaderFromFile(":/shaders/phong.fsh", GL_FRAGMENT_SHADER);
    phongProgram->link();

    leavesProgram = new GLSLProgram();
    leavesProgram->compileShaderFromFile(":/shaders/leavesShader.vert", GL_VERTEX_SHADER);
    leavesProgram->compileShaderFromFile(":/shaders/leavesShader.fsh", GL_FRAGMENT_SHADER);
    leavesProgram->link();

    terrainShader = new GLSLProgram();
    terrainShader->compileShaderFromFile(":/shaders/terrainShader.vert", GL_VERTEX_SHADER);
    terrainShader->compileShaderFromFile(":/shaders/terrainShader.fsh", GL_FRAGMENT_SHADER);
    terrainShader->link();

    program = phongProgram;

    glClearColor(0.52, 0.8, 0.92, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.setInterval(10);
    timer.start();
}

void OpenGlWidget::paintGL() {
    processCamera();

    auto ProjViewMat = mul(projMat, camera->matrix());

    leavesProgram->use();
    leavesProgram->setUniform("ProjViewMat", ProjViewMat);

    terrainShader->use();
    terrainShader->setUniform("ProjViewMat", ProjViewMat);
    terrainShader->setUniform(
            "ModelMat",
            terrain->matrix()
    );
    terrain->render();

    program->use();
    program->setUniform("ProjViewMat", ProjViewMat);

    selTex = textures[2];
    selTex->bind(0);
    program->setUniform(
            "ModelMat",
            building->matrix()
    );
    building->render();
    selTex->unbind();


    selTex = textures[0];
    selTex->bind(0);
    program->use();
    for(size_t i=0; i< trees->size(); i++) {


        program->setUniform(
                    "ModelMat",
                    trees->at(i)->getMeshTree()->matrix()
        );

        trees->at(i)->getMeshTree()->render();
    }
    selTex->unbind();


    selTex = textures[1];
    selTex->bind(0);
    leavesProgram->use();
    for(size_t i=0; i< trees->size(); i++) {
        leavesProgram->setUniform(
                    "ModelMat",
                    trees->at(i)->getMeshTwig()->matrix()
        );
        leavesProgram->setUniform("leavesColor", leavesColor);
        trees->at(i)->getMeshTwig()->render();
    }
    selTex->unbind();

}
void OpenGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projMat = perspectiveMat(60.0f, w/(float)h, 0.01f, 150.0f);
}

void OpenGlWidget::keyPressEvent(QKeyEvent *event) {
    keys.insert(event->key());
}


void OpenGlWidget::keyReleaseEvent(QKeyEvent *event) {
    keys.erase(event->key());
}

void OpenGlWidget::createNewTree(int x, int y)
{
    GLdouble projection[16];

    mat4_floatToGLdouble(projection, projMat.m);

    vec3 dir_ray = camera->mouseClickRay(x, y, projection, width, height);

    vec3 planePoint = {0, 0, 0};
    vec3 planeNormal = {0, 1, 0};
    vec3 diff = camera->pos - planePoint;
    float prod1 = dot(diff, planeNormal);
    float prod2 = dot(dir_ray, planeNormal);
    float prod3 = prod1 / prod2;
    vec3 intersecPoint = camera->pos - dir_ray * prod3;

    if(intersecPoint.x >= xPosMax || intersecPoint.x <= -xPosMax || intersecPoint.z >= zPosMax || intersecPoint.z <= -zPosMax)
        return;

    makeCurrent();

    Tree *newTree = new Tree(this->params);
    newTree->setPos(intersecPoint);
    trees->push_back(newTree);

    int treeIndex = trees->size();
    QString treeName = "Tree " + QString::number(treeIndex);
    listWidget->addItem(treeName);
    listWidget->setCurrentRow(listWidget->count() - 1);
}


void OpenGlWidget::loadFromPath(QString path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        std::string shader_src =  stream.readAll().toStdString();
        json j;
        j = json::parse(shader_src);
        Proctree::Properties props(j);
        Mesh::changeTree(*meshTree, *meshTwig, props);
    }
}

void OpenGlWidget::loadTexture(const char * path, TextureType type)
{
    switch(type)
    {
    case TextureType::WOOD:
        textures[0]->loadFromImage(path);
        break;
    case TextureType::TWIG:
        textures[1]->loadFromImage(path);
        break;
    }

}

void OpenGlWidget::loadBuildingTexture(const char *path)
{
    makeCurrent();
    textures[2]->loadFromImage(path);
    building = Mesh::createBuilding(textures[2]->width(), textures[2]->height(), 100.0f);
    building->pos = {-50, 0, -50};
}

void OpenGlWidget::mousePressEvent(QMouseEvent * e) {
    switch(e->button()) {
        case Qt::LeftButton:
            ax = e->pos().x();
            ay = e->pos().y();
        break;
        case Qt::RightButton:
            createNewTree(e->pos().x(), e->pos().y());
        break;
        default:
        break;
    }
}

void OpenGlWidget::mouseMoveEvent(QMouseEvent * e) {
    if(e->buttons() == Qt::LeftButton) {
        dax += ax-e->pos().x();
        day += ay-e->pos().y();

        ax = e->pos().x();
        ay = e->pos().y();
    }
}

void OpenGlWidget::loadSeasonValue(int val)
{
    val %= 10;
    // val 0-9
    float chVal = 0.1*val;
    float minusOneChVal = 1 - chVal;
    leavesColor = {chVal, minusOneChVal, 0, 1};
}

void OpenGlWidget::processCamera() {
    float dv = 0.1f;

    if(keys.find(Qt::Key_W) != keys.end())
        camera->pos = camera->pos + camera->forward*dv;
    else if(keys.find(Qt::Key_S) != keys.end())
        camera->pos = camera->pos - camera->forward*dv;
    if(keys.find(Qt::Key_A) != keys.end())
        camera->pos = camera->pos - camera->x()*dv;
    else if(keys.find(Qt::Key_D) != keys.end())
        camera->pos = camera->pos + camera->x()*dv;
    if(keys.find(Qt::Key_Q) != keys.end())
        camera->pos = camera->pos + camera->up*dv;
    else if(keys.find(Qt::Key_Z) != keys.end())
        camera->pos = camera->pos - camera->up*dv;

    if(camera->pos.y < 0.1f)
        camera->pos.y = 0.1f;
    else if(camera->pos.y > yPosMax)
        camera->pos.y = yPosMax;

    if(camera->pos.x < -xPosMax)
        camera->pos.x = -xPosMax;
    else if(camera->pos.x > xPosMax)
        camera->pos.x = xPosMax;

    if(camera->pos.z < -zPosMax)
        camera->pos.z = -zPosMax;
    else if(camera->pos.z > zPosMax)
        camera->pos.z = zPosMax;

    camera->forward = {0,0,-1};
    camera->forward = camera->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera->x());
    camera->forward = camera->forward * rotationMat(std::clamp<float>(day,-89.9,89.9), cx.x, cx.y, cx.z);
}

void OpenGlWidget::rotateLight() {
    lightPosition = lightPosition * rotationMat(1, 0,1,0);
    lightPosition = lightPosition * rotationMat(1, 0, 1, 0);
}

