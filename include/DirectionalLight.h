#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "Light.h"
#include <Eigen/Core>

/*
Global illumination with direction.
*/
class DirectionalLight : public Light
{
public:
    // Direction from light toward scene.
    Eigen::Vector3d light_direction;

    /* Given a query point return direction toward Light.
    
    IN
    q: 3D query point in space
    OUT
    d: direction vector from point towards light.
    max_t: parametric distance to light: infinite for a directional light
    */
    void direction(
        const Eigen::Vector3d &q, Eigen::Vector3d &d, double &max_t) const;
};
#endif
