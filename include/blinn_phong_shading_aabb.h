#ifndef BLINN_PHONG_SHADING_AABB_H
#define BLINN_PHONG_SHADING_AABB_H
#include "Ray.h"
#include "Light.h"
#include "Object.h"
#include "AABBTree.h"
#include <Eigen/Core>
#include <vector>
#include <memory>

/*
Given ray, pointer to object hit by ray & hit conditions,
and all lights in scene, returns vector3d of RGB values from
Blinn-Phong shading.

IN
- ray:  Incoming ray
- descendant: Pointer to descendant object of AABB tree that was hit
- t: t-parameter for ray to hit
- n: Unit normal vector of object surface at hit location
- root: Root of AABB tree of scene objects

OUT
- lights: All lights in the scene
*/
Eigen::Vector3d blinn_phong_shading_aabb(
    const Ray &ray,
    const std::shared_ptr<Object> &descendant,
    const double &t,
    const Eigen::Vector3d &n,
    const std::shared_ptr<AABBTree> &root,
    const std::vector<std::shared_ptr<Light>> &lights);

#endif
