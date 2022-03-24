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

void OpenGlWidget::initializeGL() {
    de = 0;
    angle = 0.0f;
    ax=ay=dax=day=0;

    lightPosition = {10, 5, 0};

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    camera = new Camera();

    Texture *treeTex, *twigTex;
    treeTex = new Texture();
    twigTex = new Texture();
    treeTex->loadFromImage(":/textures/trees/1.jpg");
    twigTex->loadFromImage(":/textures/twigs/twig1.png");
    textures.push_back(treeTex);
    textures.push_back(twigTex);

    meshTwig = new Mesh(GL_TRIANGLES);
    meshTree = Mesh::generateTree(meshTwig);
    meshTree->pos = {0, 0, 0};
    meshTwig->pos = {0, 0, 0};
    meshes.push_back(meshTree);
    meshes.push_back(meshTwig);


    phongProgram = new GLSLProgram();
    phongProgram->compileShaderFromFile(":/shaders/phong.vert", GL_VERTEX_SHADER);
    phongProgram->compileShaderFromFile(":/shaders/phong.fsh", GL_FRAGMENT_SHADER);
    phongProgram->link();

    program = phongProgram;

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.setInterval(10);
    timer.start();
}

void OpenGlWidget::paintGL() {
    processCamera();
    rotateLight();
    glClearColor(0.7, 0.7, 0.7, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    program->use();

    program->setUniform("ViewMat", camera->matrix());
    program->setUniform("ProjMat", projMat);
    program->setUniform("LightPosition", lightPosition);

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
        program->setUniform("leavesColor", colors[texInd]);
        program->setUniform("ColorTexture", 1);

        mesh->render();
        texture->unbind();
    }

}
void OpenGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    width = w;
    height = h;
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


    makeCurrent();

    Mesh *Twig = new Mesh(GL_TRIANGLES);
    Mesh *Tree = Mesh::generateTree(Twig);
    Tree->pos = intersecPoint;
    Twig->pos = intersecPoint;
    meshes.push_back(Tree);
    meshes.push_back(Twig);
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

void OpenGlWidget::loadFromJSON(json j)
{
    Proctree::Properties props(j);
    Mesh::changeTree(*meshTree, *meshTwig, props);
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

    camera->forward = {0,0,-1};
    camera->forward = camera->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera->x());
    camera->forward = camera->forward * rotationMat(std::clamp<float>(day,-89.9,89.9), cx.x, cx.y, cx.z);
}

void OpenGlWidget::rotateLight() {
    lightPosition = lightPosition * rotationMat(1, 0,1,0);
    lightPosition = lightPosition * rotationMat(1, 0, 1, 0);
}

