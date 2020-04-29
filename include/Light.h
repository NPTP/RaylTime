#ifndef LIGHT_H
#define LIGHT_H
#include <Eigen/Core>
class Light
{
public:
    // Color (intensities)
    Eigen::Vector3d I;

    virtual ~Light(){};
    /* 
    Given a query point q, return direction d toward light.
    t_to_light is parametric distance along d to the light.
    */
    virtual void direction(
        const Eigen::Vector3d &q,
        Eigen::Vector3d &d,
        double &t_to_light) const = 0;
};
#endif
