#ifndef AABBTREE_H
#define AABBTREE_H

#include "BoundingBox.h"
#include "Object.h"
#include <Eigen/Core>
#include <memory>
#include <vector>
#include "ray_intersect_box.h"
#include "rand_colour_double.h"

struct AABBTree : public Object, public std::enable_shared_from_this<AABBTree>
{
    // Pointers to left and right subtree branches, either another node of tree
    // or an object primitive
    std::shared_ptr<Object> left;
    std::shared_ptr<Object> right;
    // For AABB visualization, keep track of the depth (root depth is 0)
    int depth;
    // For debugging, keep track of the number leaf, descendants
    int num_leaves;

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
    // Object implementations (see Object.h for API)
    bool intersect(
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant) const override;
};

// Helper for AABB tree. Sets colour of bounding box for AABB visualization.
void set_random_aabb_colour(std::shared_ptr<Material> &vis_mat)
{
    std::shared_ptr<Material> new_mat(new Material());
    new_mat->ka = Eigen::Vector3d(G_rand_colour_double(), G_rand_colour_double(), G_rand_colour_double());
    new_mat->kd = Eigen::Vector3d(0.0, 0.0, 0.0);
    new_mat->ks = Eigen::Vector3d(0.0, 0.0, 0.0);
    new_mat->km = Eigen::Vector3d(0.0, 0.0, 0.0);
    new_mat->phong_exponent = 0;
    vis_mat = new_mat;
}

#endif
