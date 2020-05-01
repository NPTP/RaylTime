#ifndef BOX_INTERSECT_BOX_H
#define BOX_INTERSECT_BOX_H
#include "BoundingBox.h"

/*
Returns true if box A and box B intersect.
*/
bool box_intersect_box(
    const BoundingBox &A,
    const BoundingBox &B);
#endif
