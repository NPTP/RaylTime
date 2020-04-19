#ifndef INSERT_SPHEREANIM_INTO_BOX_H
#define INSERT_SPHEREANIM_INTO_BOX_H
#include "BoundingBox.h"
#include <Eigen/Core>

/*
Grow box B by inserting ANIMATED sphere.
Creates a box that is tall enough to encompass all possible
heights of the sphere as it moves (level_height).
*/
void insert_sphereanim_into_box(
    const Eigen::Vector3d &center,
    const double &radius,
    const int &level_height,
    BoundingBox &B);
#endif