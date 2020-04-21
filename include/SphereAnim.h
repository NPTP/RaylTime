#ifndef SPHERE_ANIM_H
#define SPHERE_ANIM_H

#include "Sphere.h"
#include "Animator.h"
#include "insert_sphereanim_into_box.h"
#include <Eigen/Core>

/* Animated Sphere with specific dimensions in constructor.  */
class SphereAnim : public Sphere, public Animator
{
public:
    double radius, max_height, min_height;
    bool moving_up, moving_down;

    // Constructor inserts the animating sphere into its own bounding box.
    SphereAnim(const Eigen::Vector3d world_center, const double radius, const int &level_height)
    {
        insert_sphereanim_into_box(
            world_center,
            radius,
            level_height,
            this->box);
        this->center = world_center;
        this->radius = radius;
        this->moving_up = true;
        this->moving_down = false;
        this->max_height = level_height - radius;
        this->min_height = radius;
    }

    bool intersect(
        const Ray& ray,
        const double min_t,
        double& t,
        Eigen::Vector3d& n,
        std::shared_ptr<Object>& descendant) const;

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
