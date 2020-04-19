#ifndef FIRST_HIT_H_AABB
#define FIRST_HIT_H_AABB

#include "AABBTree.h"
#include "Ray.h"
#include "Object.h"
#include <Eigen/Core>
#include <vector>
#include <memory>

/*
Given ray and AABB tree, find first visible hit in scene.

IN
ray: ray that was cast
min_t: minimum parametric distance to consider (should probably be at least
distance from camera to image plane)
root: root node of AABB tree
OUT
descendant: pointer to object that was hit
t: parametric distance to the hit along ray
n: normal on surface at hit
Returns true if hit, false if no hit
*/
bool first_hit_aabb(
    const Ray &ray,
    const double min_t,
    const std::shared_ptr<AABBTree> &root,
    std::shared_ptr<Object> &descendant,
    double &t,
    Eigen::Vector3d &n);

#endif
