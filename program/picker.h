#ifndef PICKER_H
#define PICKER_H
#include <glmath.h>
#include "camera.h"


class picker
{
private:
    vec3 curRay;
    mat4 projectMatrix;
    mat4 viewMatrix;
    Camera *cam;

public:
    picker(Camera* cam, mat4 projection);
    vec3 getCurrentRay();
    void update();
    vec3 calculateMouseRay();
    vec2 getNormalizedDeviceCoords(float mouseX, float mouseY);
};

#endif // PICKER_H
