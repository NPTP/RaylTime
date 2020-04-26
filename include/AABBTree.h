#ifndef AABBTREE_H
#define AABBTREE_H

#include "BoundingBox.h"
#include "Object.h"
#include <Eigen/Core>
#include <memory>
#include <vector>
#include "ray_intersect_box.h"

// TODO: return to struct (and remove access modifiers)? Or remain as class (probably better tbh)?
class AABBTree : public Object, public std::enable_shared_from_this<AABBTree>
{
    private:
        // Pointers to left and right subtree branches, either another node of tree
        // or an object primitive
        std::shared_ptr<Object> left;
        std::shared_ptr<Object> right;
        // For AABB visualization, keep track of the depth (root depth is 0)
        int depth;
        // For debugging, keep track of the number leaf, descendants
        int num_leaves;

        std::shared_ptr<AABBTree> get_shared_ptr()
        {
            return shared_from_this();
        }

        /*
        Construct a axis-aligned bounding box tree given a list of objects.

        IN
        - objects: list of objects to put into tree
        - depth (optional): depth of tree at this node. For visualization/debug.
          Set in parent when constructing child using depth + 1.

        OUT
        - num_leaves: gets set to objects.size()
        - left/right: pointers set to subtrees/leaves.
        */
        AABBTree(
            const std::vector<std::shared_ptr<Object>> &objects,
            int depth = 0);

    public:
        /*
        Return a pointer to the closest descendant of the tree hit by ray.
        Populates t (> min_t) with the parametric distance to that descendant,
        and n with the normal of the surface at the hit.
        */
        bool intersect(
            const Ray &ray,
            const double min_t,
            double &t,
            Eigen::Vector3d &n,
            std::shared_ptr<Object> &descendant) const override;
};

#endif
