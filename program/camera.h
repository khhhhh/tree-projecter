#ifndef CAMERA_H
#define CAMERA_H

#include "frame.h"

class Camera : public Frame
{
public:
    Camera();
    vec3 x() const;
    mat4 matrix() const;
    vec3 mouseClickRay(GLdouble x, GLdouble y, GLdouble *projection, int width, int height);
};

#endif // CAMERA_H
