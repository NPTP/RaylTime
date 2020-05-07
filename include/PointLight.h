#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "Light.h"
#include <Eigen/Core>
class PointLight : public Light
{
public:
    // 3d location of light
    Eigen::Vector3d p;

    PointLight(double l_radius)
    {
        this->light_radius = l_radius;
    }

    PointLight() = default;

    /*
    Given a query point return direction toward Light.
    
    IN
    q: 3D query point in space
    OUT
    d: direction vector from point towards light.
    t_to_light: parametric distance from q along d to light (may be infinite)
    */
    void direction(
        const Eigen::Vector3d &q, Eigen::Vector3d &d, double &t_to_light) const;
};
#endif
