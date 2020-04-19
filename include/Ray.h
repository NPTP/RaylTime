#ifndef RAY_H
#define RAY_H

#include <Eigen/Core>

struct Ray
{
    Eigen::Vector3d origin;

    // Not necessarily unit-length direction vector (length might
    // be up to focal distance))
    Eigen::Vector3d direction;
};

#endif
