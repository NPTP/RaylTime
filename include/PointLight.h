#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "Light.h"
#include <Eigen/Core>
class PointLight : public Light
{
public:
    Eigen::Vector3d p;

    /*
    Given a query point return direction toward Light.
    
    IN
    q: 3D query point in space
    OUT
    d: direction vector from point towards light.
    max_t: parametric distance from q along d to light (may be infinite)
    */
    void direction(
        const Eigen::Vector3d &q, Eigen::Vector3d &d, double &max_t) const;
};
#endif
