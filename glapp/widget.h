#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include "mesh.h"
#include "glslprogram.h"
#include "camera.h"

class Widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    Camera *camera;
    std::vector<Mesh *> meshes;
    GLSLProgram *program, *gourardProgram, *phongProgram;

    mat4 projMat;

    QPoint refPoint;


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void createTree();

public:
    ~Widget();
};
#endif // WIDGET_H
