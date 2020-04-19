#ifndef INSERT_TRIANGLE_INTO_BOX_H
#define INSERT_TRIANGLE_INTO_BOX_H
#include "BoundingBox.h"
#include <Eigen/Core>

/*
Grow box B by inserting a triangle with corners a, b, c (counterclockwise).
*/
void insert_triangle_into_box(
    const Eigen::Vector3d &a,
    const Eigen::Vector3d &b,
    const Eigen::Vector3d &c,
    BoundingBox &B);
#endif