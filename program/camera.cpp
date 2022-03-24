#include "camera.h"
#include "glmath.h"
#include <GL/glu.h>
#include <GL/gl.h>


Camera::Camera() {
    pos = {0,1,5};
    up = {0,1,0};
    forward = {0,0,1};
}

vec3 Camera::x() const{
    return cross(forward, up);
}


mat4 Camera::matrix() const
{
    mat4 M = identity();
    vec3 f = normal(forward);
    vec3 xx = normal(x());
    vec3 upp = normal(cross(xx, forward));
    M.m[0] = xx.x;    M.m[1] = upp.x;    M.m[2]  = -f.x;
    M.m[4] = xx.y;    M.m[5] = upp.y;    M.m[6]  = -f.y;
    M.m[8] = xx.z;    M.m[9] = upp.z;    M.m[10] = -f.z;
    M = mul(M,translationMat({-pos.x, -pos.y, -pos.z}));
    return M;
}


vec3 Camera::mouseClickRay(GLdouble mouseX, GLdouble mouseY, GLdouble *projMat, int width, int height)
{
    GLdouble retVecX, retVecY, retVecZ;
    GLint viewport[4] = {0, 0, width, height};
    GLdouble modelview[16];
    mat4_floatToGLdouble(modelview, matrix().m);

    gluUnProject( mouseX, height - mouseY, 1, modelview, projMat, viewport, &retVecX, &retVecY, &retVecZ);

    vec3 retVec = vec3{(float)retVecX, (float)retVecY, (float)retVecZ};
    retVec = retVec - pos;
    vec3 ray = normal(retVec);
    return ray;
}
