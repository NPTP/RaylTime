#ifndef AABB_VISUALIZED_H
#define AABB_VISUALIZED_H

#include "Object.h"
#include <Eigen/Core>

// TODO A(3): delete this class if you get things working without it.
// Dummy class for visualized AABBs.
class AABB_visualized : public Object
{
public:
    AABB_visualized(std::shared_ptr<Material> material)
    {
        this->material = material;
    }
    // DO NOT USE! - Dummy intersection method.
    bool intersect(
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant) const
    {
        return true;
    }
};

#endif
