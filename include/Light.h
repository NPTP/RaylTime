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
    max_t is parametric distance along d to the light.
    */
    virtual void direction(
        const Eigen::Vector3d &q,
        Eigen::Vector3d &d,
        double &max_t) const = 0;
};
#endif
