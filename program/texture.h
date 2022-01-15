#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_3_3_Core>


class Texture :  protected QOpenGLFunctions_3_3_Core
{
    GLuint handle;
public:
    static uint textsize;
    Texture();
    bool loadFromImage(const char* path);

    void bind(int unit);
    void unbind();
};

#endif // TEXTURE_H
