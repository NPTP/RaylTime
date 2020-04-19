#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Object.h"

/*
Abstract subclass of Object which allows for differentiation
of static and animating objects. In terms of implementation, the
only difference is a new animate() member function.
*/
class Animator : public Object
{
public:
    virtual void animate() = 0;
};

#endif
