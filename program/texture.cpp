#include "texture.h"
#include <QImage>
#include <QGLWidget>
#include "gldebug.h"

uint Texture::textsize = 0;
Texture::Texture()
{
    initializeOpenGLFunctions();
    glGenTextures(1, &handle);
}


bool Texture::loadFromImage(const char *path)
{
    QImage image(path);
    if( !image.isNull() )
    {
        _width = image.width();
        _height = image.height();
        image = QGLWidget::convertToGLFormat(image);
        bind(0);
        textsize++;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        //glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        unbind();
        PRINT_GL_ERRORS("Texture2D::loadFromImage()");
        return true;
    }
    else
    {
        qDebug("Texture2D::loadFromImage() - texture not loaded");
        PRINT_GL_ERRORS("Texture2D::loadFromImage()");
    }
    return false;
}

void Texture::bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, handle);
    PRINT_GL_ERRORS("Texture2D::bind()");
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::width()
{
    return _width;
}

int Texture::height()
{
    return _height;
}

