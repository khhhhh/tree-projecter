#include "mesh.h"
#include "gldebug.h"
#include <QFile>
#include <QImage>

Mesh::Mesh(GLenum primitiveType) : primitiveType(primitiveType) {
    initializeOpenGLFunctions();
    // przygotowanie vertex array object - identyfikatora dla siatki
    glGenVertexArrays(1, &vao);
}
void Mesh::setAttribute(Mesh::BufferType index, vec3 *data, int n) {
    // sprawdzenie, czy atrybut jest nowy, czy należy go nadpisać
    if(!bufferObjects.contains(index)) {
        GLuint vbo;
        // tworzenie nowego vertex buffer object dodanie jego indeksu do mapy
        glGenBuffers(1, &vbo);
        bufferObjects.insert(index, vbo);
    }

    // ustawienie aktywnego mesha
    glBindVertexArray(vao);
        // ustawienie aktywnego bufora
        glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[index]);
            // włączenie możliwości zapisu tablicy pod wskazanym indeksem
            glEnableVertexAttribArray(index);
            // przepisanie danych do bufora
            glBufferData(GL_ARRAY_BUFFER, n*sizeof(vec3), data, GL_DYNAMIC_DRAW);
            // ustawienie lokalizacji danych w buforze, dzięki temu po indeksie
            // będzie można odszukiwać zmienne w shaderze i wiadomo jakiego będą typu
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
        // rozłączenie bufora a następnie vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    PRINT_GL_ERRORS("Mesh::setAttribute");
}

void Mesh::setAttribute(Mesh::BufferType index, vec2 *data, int n) {
    if(!bufferObjects.contains(index)) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        bufferObjects.insert(index, vbo);
    }

    // ustawienie aktywnego mesha
    glBindVertexArray(vao);
        // ustawienie aktywnego bufora
        glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[index]);
            // włączenie możliwości zapisu tablicy pod wskazanym indeksem
            glEnableVertexAttribArray(index);
            // przepisanie danych do bufora
            glBufferData(GL_ARRAY_BUFFER, n*sizeof(vec2), data, GL_STATIC_DRAW);
            // ustawienie lokalizacji danych w buforze, dzięki temu po indeksie
            // będzie można odszukiwać zmienne w shaderze i wiadomo jakiego będą typu
            glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
        // rozłączenie bufora a następnie vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    PRINT_GL_ERRORS("Mesh::setAttribute");
}

// szczególny przypadek setattribute ze względu na liczbę werteksów, która potem jest potrzebna
void Mesh::setVertices(vec3 *data, int n) {
    nVerts = n;
    setAttribute(Mesh::Vertices, data, n);
}

void Mesh::setIndices(uint *data, uint n) {
    nInd = n;

    if(!bufferObjects.contains(Mesh::Indices)) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        bufferObjects.insert(Mesh::Indices, vbo);
    }

    glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[Mesh::Indices]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, n*sizeof(uint), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

// ustawienie aktywnego mesha, a następnie wyrysowanie go
void Mesh::render() {
    glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[Mesh::Indices]);
            glDrawElements(primitiveType, nInd, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glDrawArrays(primitiveType, 0, nVerts);
    glBindVertexArray(0);

    PRINT_GL_ERRORS("Mesh::render");

}

Mesh *Mesh::loadFromObj(const char *filename) {
    QFile file(filename);

    Mesh *mesh = new Mesh(GL_TRIANGLES);

    if(file.open(QFile::ReadOnly)) {
        int currentIndex=0;
        QTextStream stream(&file);
        std::vector<vec3> vertices, normals,  resultVertices, resultNormals;
        std::vector<vec2> uvs, resultUvs;
        std::vector<uint> indices;
        while(!stream.atEnd()) {
            QString line = stream.readLine();
            if(!line.isEmpty()) {
                auto le = line.split(' ');
                if(line[0]=='v') {
                    if(line[1]=='n')
                        normals.push_back(vec3{le[1].toFloat(), le[2].toFloat(), le[3].toFloat()});
                    else if(line[1]=='t')
                        uvs.push_back(vec2{le[1].toFloat(), le[2].toFloat()});
                    else
                        vertices.push_back(vec3{le[1].toFloat(), le[2].toFloat(), le[3].toFloat()});
                }
                else if(line[0]=='f') {
                    for(int i=1; i<=3; i++) {
                        auto faceData = le[i].split('/');
                        resultVertices.push_back(vertices[faceData[0].toUInt()-1]);
                        resultUvs.push_back(uvs[faceData[1].toUInt()-1]);
                        resultNormals.push_back(normals[faceData[2].toUInt()-1]);
                        indices.push_back(currentIndex++);
                    }
                }
            }
        }
        mesh->setVertices(resultVertices.data(), resultVertices.size());
        mesh->setAttribute(Mesh::Normals, resultNormals.data(), resultNormals.size());
        mesh->setAttribute(Mesh::UV, resultUvs.data(), resultUvs.size());
        mesh->setIndices(indices.data(), indices.size());
        file.close();
        qDebug() << filename << " loaded";
    }
    else
        qDebug() << filename << " loading error";
    return mesh;
}


Mesh *Mesh::createTerrain() {
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    std::vector<vec3> vertices;
    std::vector<vec2> UVs;
    std::vector<uint> indices;
    std::vector<vec3> colors;

    vertices.push_back({0,0,0});
    vertices.push_back({300,0,0});
    vertices.push_back({300,0,300});
    vertices.push_back({0,0,300});

    mesh->setVertices(vertices.data(), vertices.size());


    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);

    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);

    mesh->setIndices(indices.data(), indices.size());

    UVs.push_back({0, 0});
    UVs.push_back({1, 0});
    UVs.push_back({1, 1});
    UVs.push_back({0, 1});

    //mesh->setAttribute(Mesh::UV, UVs.data(), UVs.size());

    colors.push_back({0, 1, 0});
    colors.push_back({0, 1, 0});
    colors.push_back({0, 1, 0});
    colors.push_back({0, 1, 0});

    mesh->setAttribute(Mesh::Colors, colors.data(), colors.size());
    return mesh;
}

Mesh *Mesh::generateTree(Mesh *meshTwig) {
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    Proctree::Tree tree;
    tree.generate();
    std::vector<vec3> vertices, normals;
    std::vector<vec2> UVs;
    std::vector<uint> indices;

    std::vector<vec3> verticesTwig, normalsTwig;
    std::vector<vec2> UVsTwig;
    std::vector<uint> indicesTwig;

    for (int i = 0; i < tree.mVertCount; i++) {
        vec3 vertex = {tree.mVert[i].x, tree.mVert[i].y, tree.mVert[i].z};
        vec3 normal = {tree.mNormal[i].x, tree.mNormal[i].y, tree.mNormal[i].z};
        vec2 UV = {tree.mUV[i].u, tree.mUV[i].v};
        vertices.push_back(vertex);
        normals.push_back(normal);
        UVs.push_back(UV);
    }
    for(int i = 0; i < tree.mTwigVertCount; i++) {
        vec3 vertexTwig = {tree.mTwigVert[i].x, tree.mTwigVert[i].y,tree.mTwigVert[i].z};
        vec3 normalTwig = {tree.mTwigNormal[i].x, tree.mTwigNormal[i].y, tree.mTwigNormal[i].z};
        vec2 UV = {tree.mTwigUV[i].u, tree.mTwigUV[i].v};
        verticesTwig.push_back(vertexTwig);
        normalsTwig.push_back(normalTwig);
        UVsTwig.push_back(UV);
    }
    for (int i = 0; i < tree.mFaceCount; i++) {
        indices.push_back(tree.mFace[i].x);
        indices.push_back(tree.mFace[i].y);
        indices.push_back(tree.mFace[i].z);
    }
    for (int i = 0; i < tree.mTwigFaceCount; i++) {
        indicesTwig.push_back(tree.mTwigFace[i].x);
        indicesTwig.push_back(tree.mTwigFace[i].y);
        indicesTwig.push_back(tree.mTwigFace[i].z);
    }

    mesh->setVertices(vertices.data(), vertices.size());
    mesh->setIndices(indices.data(), indices.size());
    mesh->setAttribute(Mesh::Normals, normals.data(), normals.size());
    mesh->setAttribute(Mesh::UV, UVs.data(), UVs.size());

    meshTwig->setVertices(verticesTwig.data(), verticesTwig.size());
    meshTwig->setIndices(indicesTwig.data(), indicesTwig.size());
    meshTwig->setAttribute(Mesh::Normals, normalsTwig.data(), normals.size());
    meshTwig->setAttribute(Mesh::UV, UVsTwig.data(), UVsTwig.size());
    return mesh;
}

void Mesh::changeTree(Mesh &meshTree, Mesh &meshTwig, Proctree::Properties props) {
    Proctree::Tree tree;
    tree.mProperties = props;
    tree.generate();
    std::vector<vec3> vertices, normals;
    std::vector<vec2> UVs;
    std::vector<uint> indices;

    std::vector<vec3> verticesTwig, normalsTwig;
    std::vector<vec2> UVsTwig;
    std::vector<uint> indicesTwig;

    for (int i = 0; i < tree.mVertCount; i++) {
        vec3 vertex = {tree.mVert[i].x, tree.mVert[i].y, tree.mVert[i].z};
        vec3 normal = {tree.mNormal[i].x, tree.mNormal[i].y, tree.mNormal[i].z};
        vec2 UV = {tree.mUV[i].u, tree.mUV[i].v};
        vertices.push_back(vertex);
        normals.push_back(normal);
        UVs.push_back(UV);
    }
    for(int i = 0; i < tree.mTwigVertCount; i++) {
        vec3 vertexTwig = {tree.mTwigVert[i].x, tree.mTwigVert[i].y,tree.mTwigVert[i].z};
        vec3 normalTwig = {tree.mTwigNormal[i].x, tree.mTwigNormal[i].y, tree.mTwigNormal[i].z};
        vec2 UV = {tree.mTwigUV[i].u, tree.mTwigUV[i].v};
        verticesTwig.push_back(vertexTwig);
        normalsTwig.push_back(normalTwig);
        UVsTwig.push_back(UV);
    }
    for (int i = 0; i < tree.mFaceCount; i++) {
        indices.push_back(tree.mFace[i].x);
        indices.push_back(tree.mFace[i].y);
        indices.push_back(tree.mFace[i].z);
    }
    for (int i = 0; i < tree.mTwigFaceCount; i++) {
        indicesTwig.push_back(tree.mTwigFace[i].x);
        indicesTwig.push_back(tree.mTwigFace[i].y);
        indicesTwig.push_back(tree.mTwigFace[i].z);
    }

    meshTree.setVertices(vertices.data(), vertices.size());
    meshTree.setIndices(indices.data(), indices.size());
    meshTree.setAttribute(Mesh::Normals, normals.data(), normals.size());
    meshTree.setAttribute(Mesh::UV, UVs.data(), UVs.size());

    meshTwig.setVertices(verticesTwig.data(), verticesTwig.size());
    meshTwig.setIndices(indicesTwig.data(), indicesTwig.size());
    meshTwig.setAttribute(Mesh::Normals, normalsTwig.data(), normals.size());
    meshTwig.setAttribute(Mesh::UV, UVsTwig.data(), UVsTwig.size());
}

Mesh *Mesh::createBuilding(int width, int height,float scale)
{
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    std::vector<vec3> vertices, normals;
    std::vector<vec2> UVs;
    std::vector<uint> indices;
    float ratio;
    ratio = (float)height / width;

    vec3 verts[] = {
        {0,0,1*scale},
        {1*scale,0,1*scale},
        {1*scale,ratio*scale,1*scale},
        {0,ratio*scale,1*scale},

        {0,0,0},
        {1*scale,0,0},
        {1*scale,ratio*scale,0},
        {0,ratio*scale,0},
    };

    UVs = {
        {0.0f,0.0f},
        {1.0f,0.0f},
        {1.0f,1.0f},
        {0.0f,1.0f},

        {0.0f,0.0f},
        {1.0f,0.0f},
        {1.0f,1.0f},
        {0.0f,1.0f}

        /*
        {1.0f,1.0f},
        {0.0f,1.0f},

        {0.0f,0.0f},
        {1.0f,0.0f},
        {1.0f,1.0f},
        {0.0f,1.0f},
        {1.0f,1.0f},
        {0.0f,1.0f},
            */
    };
    for(int i = 0; i < 8; i++)
    {
        vertices.push_back(verts[i]);
        normals.push_back(verts[i]);
    }


    uint vec[] =
            /*
        {1, 2, 6,
         6, 5, 1,

         0, 4, 7,
         7, 3, 0,

         4, 5, 6,
         6, 7, 4,

         0, 3, 2,
         2, 1, 0,

         0, 1, 5,
         5, 4, 0,

         3, 7, 6,
         6, 2, 3 };
                 */
    {
        0, 1, 2,
        0, 2, 3,

        1, 5, 6,
        1, 6, 2,

        4, 0, 3,
        4, 3, 7,

        3, 2, 6,
        3, 6, 7,

        5, 4, 6,
        4, 7, 6
    };
    for(int i = 0; i < 30; i++)
        indices.push_back(vec[i]);

    mesh->setVertices(vertices.data(), vertices.size());
    mesh->setIndices(indices.data(), indices.size());
    mesh->setAttribute(Mesh::Normals, normals.data(), normals.size());
    mesh->setAttribute(Mesh::UV, UVs.data(), UVs.size());

    return mesh;
}


