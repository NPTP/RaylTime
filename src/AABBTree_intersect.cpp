#include "AABBTree.h"
#include <limits>
#include "Sphere.h"
#include "globals.h"

bool AABBTree::intersect(
    const Ray& ray,
    const double min_t,
    double & t,
    Eigen::Vector3d & n,
    std::shared_ptr<Object> & descendant)
{
    bool ray_intersected_box = ray_intersect_box(ray, this->box, min_t, std::numeric_limits<double>::infinity());
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
    // Use these casts to test if children are AABBTrees, or if they are primitives.
    std::shared_ptr<AABBTree> left_is_aabb = std::dynamic_pointer_cast<AABBTree>(this->left);
    std::shared_ptr<AABBTree> right_is_aabb = std::dynamic_pointer_cast<AABBTree>(this->right);

    // Set up for recursive intersection tests.
    double left_t, right_t;
    bool left_hit  = false;
    bool right_hit = false;
    Eigen::Vector3d left_n;
    Eigen::Vector3d right_n;
    std::shared_ptr<Object> left_descendant, right_descendant;

    // Run through all checks of intersection.
    if (this->left) {
      left_hit = this->left->intersect(ray, min_t, left_t, left_n, left_descendant);
    }
    if (this->right) {
      right_hit = this->right->intersect(ray, min_t, right_t, right_n, right_descendant);
    }

    if (left_hit && right_hit) {
      if (left_t < right_t) {
        t = left_t;
        n = left_n;
        if (left_is_aabb) {
          descendant = left_descendant;
        }
        else {
          descendant = this->left;
        }
      }
      else {
        t = right_t;
        n = right_n;
        if (right_is_aabb) {
          descendant = right_descendant;
        }
        else {
          descendant = this->right;
        }
      }
      return true;
    }
    else if (left_hit) {
      t = left_t;
      n = left_n;
      if (left_is_aabb) {
        descendant = left_descendant;
      }
      else {
        descendant = this->left;
      }
      return true;
    }
    else if (right_hit) {
      t = right_t;
      n = right_n;
      if (right_is_aabb) {
        descendant = right_descendant;
      }
      else {
        descendant = this->right;
      }
      return true;
    }
    else {
      return false;
    }

}
