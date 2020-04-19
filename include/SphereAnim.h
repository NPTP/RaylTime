#ifndef SPHERE_ANIM_H
#define SPHERE_ANIM_H

#include "Animator.h"
#include "insert_sphereanim_into_box.h"
#include <Eigen/Core>

/* Animated version of Sphere. */
class SphereAnim : public Animator
{
public:
    Eigen::Vector3d center;
    double radius, max_height, min_height;
    bool moving_up, moving_down;

    // Constructor inserts the animating sphere into its own bounding box.
    SphereAnim(const Eigen::Vector3d center, const double radius, const int &level_height)
    {
        insert_sphereanim_into_box(
            center,
            radius,
            level_height,
            this->box);
        this->center = center;
        this->radius = radius;
        this->moving_up = true;
        this->moving_down = false;
        this->max_height = level_height - radius;
        this->min_height = radius;
    }

    /*
    Given ray and min_t minimum parametric distance to consider,
    change t to parametric distance to intersection, n to normal on
    animated sphere at point of intersection.
    Return true if there is an intersection, false if not.
    */
    bool intersect(
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant) const;

    /*
    Animate the sphere! No arguments, just moves the sphere up and down
    according to the max heights stashed earlier on construction based
    on level height.
    */
    void animate()
    {
        if (moving_up)
        {
            if (this->center[1] < max_height)
                this->center[1] += 0.1;
            else
            {
                moving_up = false;
                moving_down = true;
            }
        }
        else if (moving_down)
        {
            if (this->center[1] > min_height)
                this->center[1] -= 0.1;
            else
            {
                moving_down = false;
                moving_up = true;
            }
        }
    }
};

#endif
