#include "camera.h"

Camera::Camera() {
    pos = {0,1,5};
    up = {0,1,0};
    forward = {0,0,1};
}

vec3 Camera::x() const{
    return cross(forward, up);
}

//mat4 Camera::matrix() const {
//    mat4 result = identity();
//    vec3 vecX = x();
//    vec3 upp = cross(vecX, forward);
//    result.m[0] = vecX.x;    result.m[1] = upp.x;    result.m[2]  = -forward.x;
//    result.m[4] = vecX.y;    result.m[5] = upp.y;    result.m[6]  = -forward.y;
//    result.m[8] = vecX.z;    result.m[9] = upp.z;    result.m[10] = -forward.z;

//    result = mul(result, translationMat(vec3{-pos.x,-pos.y,-pos.z}));

////    if(parent)
////        result = mul(parent->matrix(), result);

//    return result;
//}


mat4 Camera::matrix(/*bool rotationOnly = false*/) const
{
//    mat4 M = identity();
//    vec3 f = normal(forward);
//    vec3 xx = x();
//    normalize(xx);
//    vec3 upp = cross(xx, forward);
//    normalize(upp);
//    M.m[0] = xx.x;    M.m[1] = upp.x;    M.m[2]  = -f.x;
//    M.m[4] = xx.y;    M.m[5] = upp.y;    M.m[6]  = -f.y;
//    M.m[8] = xx.z;    M.m[9] = upp.z;    M.m[10] = -f.z;

//    M = mul(M,translationMat({-pos.x, -pos.y, -pos.z}));

//    return M;

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
