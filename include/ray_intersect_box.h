#ifndef RAY_INTERSECT_BOX_H
#define RAY_INTERSECT_BOX_H
#include "BoundingBox.h"
#include "Ray.h"
#include <Eigen/Core>

/*
Given a ray and a box, return true if the ray intersects the box
between min_t and max_t.
*/
bool ray_intersect_box(
    const Ray &ray,
    const BoundingBox &box,
    const double min_t,
    const double max_t);
#endif
