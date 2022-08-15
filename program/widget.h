#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <QListWidget>

#include "mesh.h"
#include "glslprogram.h"
#include "camera.h"
#include <set>
#include "texture.h"
#include "tree.h"

enum TextureType { WOOD, TWIG };

class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    QTimer timer;
    Camera *camera;
    GLSLProgram *program, *phongProgram, *leavesProgram, *terrainShader;
    std::vector<Texture *> textures;

    Texture *selTex;

    Mesh *terrain;
    Mesh *building;
    mat4 projMat;

    int de;
    float angle;
    float ax,ay,dax,day;
    vec3 lightPosition;
    vec4 leavesColor;


    void processCamera();
    void rotateLight();
    void switchProgram();

    void createNewTree(int x, int y);


    //temp
    int width;
    int height;

    const static int yPosMax = 30;
    const static int zPosMax = 49;
    const static int xPosMax = 49;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    std::set<int> keys;
    std::vector<Tree *> *trees;
    QListWidget *listWidget;
    json params;

    void loadSeasonValue(int val);
    void loadTexture(const char * path, TextureType type);
    void loadBuildingTexture(const char * path);
    ~OpenGlWidget();

    void timerStop();
    void timerStart();
    void updateOpenGl();
    void growTree(int index, int procent);
};
#endif // WIDGET_H
