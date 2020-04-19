#ifndef INSERT_BOX_INTO_BOX_H
#define INSERT_BOX_INTO_BOX_H
#include "BoundingBox.h"
#include <Eigen/Core>

/*
Insert box A into box B (shrink or grow as necessary)
*/
void insert_box_into_box(
    const BoundingBox &A,
    BoundingBox &B);
#endif
