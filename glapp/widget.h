#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>

#include "mesh.h"
#include "glslprogram.h"
#include "camera.h"
#include <set>
#include "texture.h"

class Widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    QTimer timer;
    Camera *camera;
    std::vector<Mesh *> meshes;
    GLSLProgram *program, *gourardProgram, *phongProgram;
    std::vector<Texture *> textures;

    mat4 projMat;

    QPoint refPoint;

    std::set<int> keys;

    int de;
    float angle;
    float ax,ay,dax,day;
    vec3 lightPosition;


    void processCamera();
    void rotateLight();
    void switchProgram();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    ~Widget();
};
#endif // WIDGET_H
