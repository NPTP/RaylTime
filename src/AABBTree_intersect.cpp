#include "AABBTree.h"
#include "globals.h"

bool AABBTree::intersect(
    const bool &is_from_viewer,
    const Ray &ray,
    const double min_t,
    double &t,
    Eigen::Vector3d &n,
    std::shared_ptr<Object> &descendant)
{
    bool ray_intersected_box = ray_intersect_box(is_from_viewer, ray, this->box, min_t, G_max_t_draw_distance);
    if (!ray_intersected_box)
        return false;

    // Visualization of AABB Tree. Skips the rest of tree traversal logic.
    if (G_show_boxes && this->depth == G_show_boxes_depth)
    {
        t = 1;
        n = -ray.direction;
        descendant = this->get_shared_ptr();
        return true;
    }
    // Standard behaviour of tree from hereon out.

    // Set up for recursive intersection tests.
    double left_t, right_t;
    bool left_hit = false;
    bool right_hit = false;
    Eigen::Vector3d left_n;
    Eigen::Vector3d right_n;
    std::shared_ptr<Object> left_descendant, right_descendant;

    // Run through all checks of intersection.
    if (this->left)
        left_hit = this->left->intersect(is_from_viewer, ray, min_t, left_t, left_n, left_descendant);
    if (this->right)
        right_hit = this->right->intersect(is_from_viewer, ray, min_t, right_t, right_n, right_descendant);

    if (left_hit && right_hit)
    {
        if (left_t < right_t)
        {
            t = left_t;
            n = left_n;
            if (std::dynamic_pointer_cast<AABBTree>(this->left))
                descendant = left_descendant;
            else
                descendant = this->left;
        }
        else
        {
            t = right_t;
            n = right_n;
            if (std::dynamic_pointer_cast<AABBTree>(this->right))
                descendant = right_descendant;
            else
                descendant = this->right;
        }
        return true;
    }
    else if (left_hit)
    {
        t = left_t;
        n = left_n;
        if (std::dynamic_pointer_cast<AABBTree>(this->left))
            descendant = left_descendant;
        else
            descendant = this->left;
        return true;
    }
    else if (right_hit)
    {
        t = right_t;
        n = right_n;
        if (std::dynamic_pointer_cast<AABBTree>(this->right))
            descendant = right_descendant;
        else
            descendant = this->right;
        return true;
    }
    else
    {
        return false;
    }
}
