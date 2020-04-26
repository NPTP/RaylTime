#include "first_hit_aabb.h"
#include <limits>

// TODO: remove this entirely (and its header file) if not used anymore
bool first_hit_aabb(
    const Ray &ray,
    const double min_t,
    const std::shared_ptr<AABBTree> &root,
    std::shared_ptr<Object> &descendant,
    double &t,
    Eigen::Vector3d &n)
{
    bool dummy = false;
    return root->intersect(ray, min_t, t, n, descendant);
}
