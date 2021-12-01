#include "widget.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>


Widget::~Widget()
{
}

void Widget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    camera = new Camera();

    //Mesh *m;

    //    m = Mesh::loadFromObj(":/monkey.obj");
    //    m->pos = {-2, 0, -1};
    //    meshes.push_back(m);

    //    m->material.ambient = vec3{ 0.329412f, 0.223529f, 0.027451f };
    //    m->material.diffuse = vec3{ 0.780392f, 0.568627f, 0.113725f };
    //    m->material.specular = vec3{ 0.992157f, 0.941176f, 0.807843f };
    //    m->material.shiness = 27.8974f;

    //m = Mesh::loadFromObj("monkey_s.obj");
    //    m = Mesh::loadFromObj(":/cube.obj");
    //    m->pos = {1, 0, -3};
    //    meshes.push_back(m);

    //    m = Mesh::loadFromObj(":/ala.obj");
    //    m->pos = {0, 0, -5};
    //    meshes.push_back(m);

    createTree();

    gourardProgram = new GLSLProgram();
    gourardProgram->compileShaderFromFile(":/vshader.vert", GL_VERTEX_SHADER);
    gourardProgram->compileShaderFromFile(":/fshader.fsh", GL_FRAGMENT_SHADER);
    gourardProgram->link();

    phongProgram = new GLSLProgram();
    phongProgram->compileShaderFromFile("phong.vert", GL_VERTEX_SHADER);
    phongProgram->compileShaderFromFile("phong.fsh", GL_FRAGMENT_SHADER);
    phongProgram->link();

    program = gourardProgram;
}
void Widget::paintGL() {
    // ustawienie koloru tła na biały
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    program->use();

    program->setUniform("ViewMat", camera->matrix());
    program->setUniform("ProjMat", projMat);
    program->setUniform("LightPosition", vec3{0,2,0});



    for(size_t i=0; i<meshes.size(); i++) {
        Mesh *mesh = meshes[i];
        program->setUniform("ModelMat", mesh->matrix());

        program->setUniform("MaterialAmbient", mesh->material.ambient);
        program->setUniform("MaterialDiffuse", mesh->material.diffuse);
        program->setUniform("MaterialSpecular", mesh->material.specular);
        program->setUniform("MaterialShiness", mesh->material.shiness);

        mesh->render();
    }

}
void Widget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    projMat = perspectiveMat(60.0f, w/(float)h, 0.01f, 15.0f);
}

void Widget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_A:
        meshes[0]->pos = meshes[0]->pos + vec3{-0.1,0,0};
        //tMat = mul(tMat, translationMat({-0.1, 0, 0}));
        break;
    case Qt::Key_D:
        meshes[0]->pos = meshes[0]->pos + vec3{0.1,0,0};
        //tMat = mul(tMat, translationMat({0.1, 0, 0}));
        break;

    case Qt::Key_W:
        camera->pos = camera->pos + camera->forward * 0.1;
        break;
    case Qt::Key_Tab:
        if(program == gourardProgram) {
            program = phongProgram;
            qDebug() << "phong";
        }
        else {
            program = gourardProgram;
            qDebug() << "gourard";
        }
        break;
    case Qt::Key_Space:
        break;
    }
    update();
}

void Widget::mousePressEvent(QMouseEvent *event) {
    refPoint = event->pos();
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    int anglex = event->x() - refPoint.x();
    int angley = event->y() - refPoint.y();

    anglex%=360;
    angley%=360;

    camera->up=camera->up*rotationMat(anglex,0,1,0)*rotationMat(angley,1,0,0);
    camera->forward=camera->forward*rotationMat(anglex,0,1,0)*rotationMat(angley,1,0,0);

    refPoint = event->pos();

    //    rMat = identity();
    //    rMat = mul(rMat, rotationMat(event->x(), 0, 1, 0));
    //    rMat = mul(rMat, rotationMat(event->y(), 1, 0, 0));
    update();
}

void Widget::createTree()
{
    Mesh* m;
    m = Mesh::generateTree();
    m->pos = {2, 0, -1};
    m->material.ambient = vec3{ 0.329412f, 0.223529f, 0.027451f };
    m->material.diffuse = vec3{ 0.780392f, 0.568627f, 0.113725f };
    m->material.specular = vec3{ 0.992157f, 0.941176f, 0.807843f };
    m->material.shiness = 27.8974f;
    meshes.push_back(m);
}


