#ifndef INSERT_QUAD_INTO_BOX_H
#define INSERT_QUAD_INTO_BOX_H
#include "BoundingBox.h"
#include <Eigen/Core>

/*
Grow box B by inserting a quad with bottom left corner 'a',
bottom right corner 'b', and top left corner 'c'.

IN
a: bottom left corner of the quad/parallelogram
b: bottom right corner of the quad/parallelogram
c: top left corner of the quad/parallelogram
B: bounding box
OUT
B: bounding box grown to include original contents and quad/parallelogram
*/
void insert_quad_into_box(
    const Eigen::Vector3d &a,
    const Eigen::Vector3d &b,
    const Eigen::Vector3d &c,
    BoundingBox &B);
#endif