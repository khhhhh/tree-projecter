#include "mesh.h"
#include "gldebug.h"
#include "proctree.h"
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
            glBufferData(GL_ARRAY_BUFFER, n*sizeof(vec3), data, GL_STATIC_DRAW);
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

//Mesh *Mesh::loadFromObj(const char *filename) {
//    QFile file(filename);
//    Mesh *mesh = new Mesh(GL_TRIANGLES);
//    //mesh->name = filename;
//    if(file.open(QFile::ReadOnly)) {
//        int currentIndex=0;
//        QTextStream stream(&file);
//        std::vector<vec3> vertices, normals, resultVertices, resultNormals;
//        std::vector<uint> indices;
//        while(!stream.atEnd()) {
//            QString line = stream.readLine();
//            if(!line.isEmpty()) {
//                auto listElements = line.split(' ');
//                if(line[0]=='v') {
//                    vec3 vertex = {listElements[1].toFloat(), listElements[2].toFloat(), listElements[3].toFloat()};
//                    if(line[1]=='n')
//                        normals.push_back(vertex);
//                    else
//                        vertices.push_back(vertex);
//                }
//                else if(line[0]=='f') {
//                    for(int i=1; i<=3; i++) {
//                        auto faceData = listElements[i].split('/');
//                        resultVertices.push_back(vertices[faceData[0].toUShort()-1]);
//                        resultNormals.push_back(normals[faceData[2].toUShort()-1]);
//                        indices.push_back(currentIndex++);
//                    }
//                }
//            }
//        }
//        mesh->setVertices(resultVertices.data(), resultVertices.size());
//        mesh->setAttribute(Mesh::Normals, resultNormals.data(), resultNormals.size());
//        mesh->setIndices(indices.data(), indices.size());
//        file.close();
//        qDebug() << filename << " loaded";
//    }
//    else
//        qDebug() << filename << " loading error";
//    return mesh;
//}

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

Mesh *Mesh::createTerrain(QImage img, vec3 scale) {
    int w = img.width();
    int h = img.height();

    Mesh *mesh = new Mesh(GL_TRIANGLES);
    std::vector<vec3> vertices(w*h);

    int i = 0;
    for(int z=0; z<h; z++)
        for(int x=0; x<w; x++)
            vertices[i++] = vec3{x*scale.x, qRed(img.pixel(x,z))*scale.y , z*scale.z};

    int iout=0, iin=0;

    mesh->setVertices(vertices.data(), vertices.size());

    std::vector<uint> indices(w*h*3);
    for(int z=0; z<h-1; z++) {
        for(int x=0; x<w-1; x++) {
            indices[iout] = iin;
            indices[iout+1] = iin + w;
            indices[iout+2] = iin + w+1;

            indices[iout+3] = iin;
            indices[iout+4] = iin + w + 1;
            indices[iout+5] = iin + 1;

            iin++;
            iout+=6;
        }
        iin++;
    }
    mesh->setIndices(indices.data(), indices.size());

    std::vector<vec3> normals(w*h);
    for(uint i=0;i<normals.size(); ++i)
        normals[i] = vec3{0,1,0};

    vec3 vup, vleft, vright, vdown, vsl, vsr;
    vec3 n[6];

    i = w + 1;

    for(int z=1; z<h-1; z++) {
        for(int x=1; x<w-1; x++) {
            vleft = vertices[i] - vertices[i-1];
            vsl = vertices[i - w - 1] - vertices[i];
            n[0] = cross(vleft, vsl);

            vup = vertices[i-w] - vertices[i];
            n[1] = cross(vup,vsl);

            vright = vertices[i+1] - vertices[i];
            n[2] = cross(vright, vup);

            vdown = vertices[i] - vertices[i+w];
            n[3] = cross(vleft, vdown);

            vsr = vertices[i] - vertices[i+w+1];
            reflect(vdown);
            n[4] = cross(vsr, vdown);

            n[5] = cross(vright, vsr);

            normalize(n[0]);

            for(int i=1; i<6; i++) {
                normalize(n[i]);
                n[0] = n[0] + n[i];
            }
            n[0] = n[0] * (1/6.f);

            normalize(n[0]);
            normals[i++] = n[0];
        }
        i+=2;
    }

    mesh->setAttribute(Mesh::Normals, normals.data(), normals.size());

    return mesh;
}

Mesh *Mesh::generateTree(Mesh &meshTwig) {
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

    meshTwig.setVertices(verticesTwig.data(), verticesTwig.size());
    meshTwig.setIndices(indicesTwig.data(), indicesTwig.size());
    meshTwig.setAttribute(Mesh::Normals, normalsTwig.data(), normals.size());
    meshTwig.setAttribute(Mesh::UV, UVsTwig.data(), UVsTwig.size());
    return mesh;
}

