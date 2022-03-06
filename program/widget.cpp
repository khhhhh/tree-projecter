#include "widget.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFile>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

OpenGlWidget::~OpenGlWidget()
{
}

void OpenGlWidget::initializeGL() {
    de = 0;
    angle = 0.0f;
    ax=ay=dax=day=0;

    lightPosition = {10, 5, 0};

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    camera = new Camera();

    meshTwig = new Mesh(GL_TRIANGLES);

    Texture *treeTex, *twigTex;
    treeTex = new Texture();
    twigTex = new Texture();
    treeTex->loadFromImage(":/textures/trees/3.jpg");
    twigTex->loadFromImage(":/textures/twigs/twig1.png");
    textures.push_back(treeTex);
    textures.push_back(twigTex);

    meshTree = Mesh::generateTree(*meshTwig);
    meshTree->pos = {0, -1, -1};
    meshTwig->pos = {0, -1, -1};
    meshes.push_back(meshTree);
    meshes.push_back(meshTwig);

    meshTwig->material.shiness = 27.8974f;

    /*
    gourardProgram = new GLSLProgram();
    gourardProgram->compileShaderFromFile(":/shaders/vshader.vert", GL_VERTEX_SHADER);
    gourardProgram->compileShaderFromFile(":/shaders/fshader.fsh", GL_FRAGMENT_SHADER);
    gourardProgram->link();
    */

    phongProgram = new GLSLProgram();
    phongProgram->compileShaderFromFile(":/shaders/phong.vert", GL_VERTEX_SHADER);
    phongProgram->compileShaderFromFile(":/shaders/phong.fsh", GL_FRAGMENT_SHADER);
    phongProgram->link();

    program = phongProgram; //gourardProgram;

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.setInterval(10);
    timer.start();
}

void OpenGlWidget::paintGL() {
    processCamera();
    rotateLight();
    glClearColor(0.7, 0.7, 0.7, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->use();

    program->setUniform("ViewMat", camera->matrix());
    program->setUniform("ProjMat", projMat);
    program->setUniform("LightPosition", lightPosition);
    //program->setUniform("LightPower", 50);

    vec4 treeCol = {1,1,1,1};
    vec4 *colors = new vec4[2];
    colors[0] = treeCol;
    colors[1] = leavesColor;

    for(size_t i=0; i< meshes.size(); i++) {
        Mesh *mesh = meshes[i];
        int texInd = i % 2;
        Texture *texture = textures[texInd];
        program->setUniform("ModelMat", mesh->matrix());
        texture->bind(0);
        //        program->setUniform("MaterialAmbient", mesh->material.ambient);
        //        program->setUniform("MaterialDiffuse", mesh->material.diffuse);
        //        program->setUniform("MaterialSpecular", mesh->material.specular);
        //program->setUniform("MaterialShiness", mesh->material.shiness);
        program->setUniform("leavesColor", colors[texInd]);
        program->setUniform("ColorTexture", 1);

        mesh->render();
        texture->unbind();
    }
}
void OpenGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    projMat = perspectiveMat(60.0f, w/(float)h, 0.01f, 150.0f);
}

void OpenGlWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Tab: switchProgram(); break;
    default: keys.insert(event->key());
    }
}


void OpenGlWidget::keyReleaseEvent(QKeyEvent *event) {
    keys.erase(event->key());
}

void OpenGlWidget::switchProgram() {
    if(program == gourardProgram) {
        program = phongProgram;
        qDebug() << "phong";
    }
    else {
        program = gourardProgram;
        qDebug() << "gourard";
    }
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

void OpenGlWidget::loadTexture(const char * path)
{
    textures[0]->loadFromImage(path);
}

void OpenGlWidget::loadFromJSON(json j)
{
    Proctree::Properties props(j);
    Mesh::changeTree(*meshTree, *meshTwig, props);
}

void OpenGlWidget::mousePressEvent(QMouseEvent * e) {
    if(e->button() == Qt::LeftButton){
        e->pos().x();
        ax = e->pos().x();
        ay = e->pos().y();
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

    camera->forward = {0,0,-1};
    camera->forward = camera->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera->x());
    camera->forward = camera->forward * rotationMat(std::clamp<float>(day,-89.9,89.9), cx.x, cx.y, cx.z);
}

void OpenGlWidget::rotateLight() {
    lightPosition = lightPosition * rotationMat(1, 0,1,0);
    lightPosition = lightPosition * rotationMat(1, 0, 1, 0);
}

