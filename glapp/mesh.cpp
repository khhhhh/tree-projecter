#include "mesh.h"
#include "gldebug.h"
#include <QFile>
#include "proctree.h"

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
            glBufferData(GL_ARRAY_BUFFER, n*sizeof(vec3), data, GL_STATIC_DRAW);
            // ustawienie lokalizacji danych w buforze, dzięki temu po indeksie
            // będzie można odszukiwać zmienne w shaderze i wiadomo jakiego będą typu
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
    //mesh->name = filename;
    if(file.open(QFile::ReadOnly)) {
        int currentIndex=0;
        QTextStream stream(&file);
        std::vector<vec3> vertices, normals, resultVertices, resultNormals;
        std::vector<uint> indices;
        while(!stream.atEnd()) {
            QString line = stream.readLine();
            if(!line.isEmpty()) {
                auto listElements = line.split(' ');
                if(line[0]=='v') {
                    vec3 vertex = {listElements[1].toFloat(), listElements[2].toFloat(), listElements[3].toFloat()};
                    if(line[1]=='n')
                        normals.push_back(vertex);
                    else
                        vertices.push_back(vertex);
                }
                else if(line[0]=='f') {
                    for(int i=1; i<=3; i++) {
                        auto faceData = listElements[i].split('/');
                        resultVertices.push_back(vertices[faceData[0].toUShort()-1]);
                        resultNormals.push_back(normals[faceData[2].toUShort()-1]);
                        indices.push_back(currentIndex++);
                    }
                }
            }
        }
        mesh->setVertices(resultVertices.data(), resultVertices.size());
        mesh->setAttribute(Mesh::Normals, resultNormals.data(), resultNormals.size());
        mesh->setIndices(indices.data(), indices.size());
        file.close();
        qDebug() << filename << " loaded";
    }
    else
        qDebug() << filename << " loading error";
    return mesh;
}

Mesh *Mesh::generateTree() {
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    Proctree::Tree tree;
    tree.generate();

    std::vector<vec3> vertices, normals;
    std::vector<uint> indices;

    for (int i = 0; i < tree.mVertCount; i++) {
        vec3 vertex = {tree.mVert[i].x, tree.mVert[i].y, tree.mVert[i].z};
        vec3 normal = {tree.mNormal[i].x, tree.mNormal[i].y, tree.mNormal[i].z};
        vertices.push_back(vertex);
        normals.push_back(normal);
    }
    for (int i = 0; i < tree.mFaceCount; i++) {
        indices.push_back(tree.mFace[i].x);
        indices.push_back(tree.mFace[i].y);
        indices.push_back(tree.mFace[i].z);
    }
    mesh->setVertices(vertices.data(), vertices.size());
    mesh->setIndices(indices.data(), indices.size());
    return mesh;
}

//Mesh *Mesh::loadFromObj(const char *filename) {
//    QFile file(filename);
//    Mesh *mesh = new Mesh(GL_TRIANGLES);
//    //mesh->name = filename;
//    if(file.open(QFile::ReadOnly)) {
//        QTextStream stream(&file);
//        std::vector<vec3> vertices;
//        std::vector<ushort> indices;
//        while(!stream.atEnd()) {
//            QString line = stream.readLine();
//            if(!line.isEmpty()) {
//                auto listElements = line.split(' ');
//                if(line[0]=='v') {
//                    vec3 vertex = {listElements[1].toFloat(), listElements[2].toFloat(), listElements[3].toFloat()};
//                    vertices.push_back(vertex);
//                }
//                else if(line[0]=='f') {
//                    indices.push_back(listElements[1].toUShort()-1);
//                    indices.push_back(listElements[2].toUShort()-1);
//                    indices.push_back(listElements[3].toUShort()-1);
//                }
//            }
//        }
//        mesh->setVertices(vertices.data(), vertices.size());
//        mesh->setIndices(indices.data(), indices.size());
//        file.close();
//        qDebug() << filename << " loaded";
//    }
//    else
//        qDebug() << filename << " loading error";
//    return mesh;
//}
