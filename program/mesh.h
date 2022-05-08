#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions_3_3_Core>
#include "glmath.h"
#include "frame.h"
#include "proctree.h"

class Mesh : public Frame, protected QOpenGLFunctions_3_3_Core{
public:
    enum BufferType {Vertices=0, Indices=1, Colors=2, Normals=3, UV=4};

    struct Material {
        vec3 ambient, diffuse, specular;
        float shiness;

        Material() {
            ambient = {0.5, 0.5, 0.5};
            diffuse = {0.5, 0.5, 0.5};
            specular = {0.5, 0.5, 0.5};
            shiness = 50;
        }
    };

private:
    QString name;
    uint nVerts, nInd;
    GLuint vao;
    GLenum primitiveType;
    QMap<BufferType, GLuint> bufferObjects;

public:
    Mesh(GLenum primitiveType);
    void setAttribute(Mesh::BufferType, vec3 *data, int n);
    void setAttribute(Mesh::BufferType, vec2 *data, int n);

    void setVertices(vec3 *data, int n);
    void setIndices(uint *data, uint n);
    void render();

    static Mesh *loadFromObj(const char *filename);
    static Mesh *generateTree(Mesh *twigMesh);
    static void changeTree(Mesh& meshTree, Mesh &meshTwig, Proctree::Properties props);
    static Mesh *generateWall(float scale);

    static Mesh *createTerrain(QImage img, vec3 scale);

    Material material;
};

#endif // MESH_H
