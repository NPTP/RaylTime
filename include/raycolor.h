#ifndef RAYCOLOR_H
#define RAYCOLOR_H
#include "Ray.h"
#include "Object.h"
#include "Light.h"
#include "AABBTree.h"
#include <Eigen/Core>
#include <vector>

/*
Shoot a ray into a lit scene and collect color information into rgb vector.
Returns true if a hit was found.
*/
bool raycolor(
    const Ray &ray,
    const double min_t,
    const std::shared_ptr<AABBTree> &root,
    const std::vector<std::shared_ptr<Light>> &lights,
    const int num_recursive_calls,
    Eigen::Vector3d &rgb);

#endif
