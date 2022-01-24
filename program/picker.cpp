#include "picker.h"
#include "camera.h"


picker::picker(Camera* cam, mat4 projection)
{
    /*
    this->cam = cam;
    this->projectMatrix = projection;
    this->viewMatrix = createViewMatrix(cam);
    */
}

vec3 picker::getCurrentRay()
{
    return this->curRay;
}

void picker::update()
{
    //this->viewMatrix = createViewMatrix(cam);
    curRay = calculateMouseRay();
}

vec3 picker::calculateMouseRay()
{
    /*
    float mouseX = Mouse.getX();
    float mouseY = Mouse.getY();
    */
}

vec2 getNormalizedDeviceCoords(float mouseX, float mouseY)
{
    /*
    float x = (2f*mouseX) / Display.getWidth() - 1;
    float y = (2f*mouseY) / Display.getHeight() - 1f;
    */

    //return new vec2(x,y);
    return {2.0,2.0};
}
