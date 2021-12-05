#ifndef FRAME_H
#define FRAME_H

#include "glmath.h"
#include <vector>

class Frame
{
protected:
    Frame *parent;
public:
    vec3 pos, up, forward;
    virtual vec3 x() const;
    virtual mat4 matrix() const;

    Frame();
    void attachTo(Frame *);
};

#endif // FRAME_H
