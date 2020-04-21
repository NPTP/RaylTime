#ifndef ANIMATOR_H
#define ANIMATOR_H

/*
Abstract class allowing for differentiation of static and animating objects.
However, does not inherit from Object: meant to be used in parallel inheritance.
Simply adds an animate() member function to children.
*/
class Animator
{
public:
    virtual void animate() = 0;
};

#endif
