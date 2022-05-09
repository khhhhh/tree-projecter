#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_3_3_Core>


class Texture :  protected QOpenGLFunctions_3_3_Core
{
    GLuint handle;
    int _width;
    int _height;
    void Release();
public:
    static uint textsize;
    Texture();
    bool loadFromImage(const char* path);

    void bind(int unit);
    void unbind();

    int width();
    int height();

    //Free up the texture.
    ~Texture();

    //Delete the copy constructor/assignment.
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture(Texture &&other) : handle(other.handle)
    {
        other.handle = 0; //Use the "null" texture for the old object.
    }

    Texture &operator=(Texture &&other)
    {
        //ALWAYS check for self-assignment.
        if(this != &other)
        {
            Release();
            //obj_ is now 0.
            std::swap(handle, other.handle);
        }
    }
};

#endif // TEXTURE_H
