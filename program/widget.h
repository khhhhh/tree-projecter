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
#include "tree.h"

enum TextureType { WOOD, TWIG };

class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    QTimer timer;
    Camera *camera;
    std::vector<Mesh *> meshes;
    GLSLProgram *program, *gourardProgram, *phongProgram;
    std::vector<Texture *> textures;
    Mesh *meshTree, *meshTwig;


    Tree *drawingTree;
    Texture *selTex;
    Mesh *treeMesh;
    Mesh *twigMesh;

    Mesh *terrain;
    mat4 projMat;

    QPoint refPoint;


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

    void loadSeasonValue(int val);
    void loadFromPath(QString path);
    void loadTexture(const char * path, TextureType type);
    ~OpenGlWidget();
};
#endif // WIDGET_H
