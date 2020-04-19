#ifndef INSERT_SPHERE_INTO_BOX_H
#define INSERT_SPHERE_INTO_BOX_H
#include "BoundingBox.h"
#include <Eigen/Core>

/*
Grow box B by inserting sphere. Use center & radius of sphere
and box B will grow to encompass it.
*/
void insert_sphere_into_box(
    const Eigen::Vector3d &center,
    const double &radius,
    BoundingBox &B);
#endif