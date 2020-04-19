#ifndef TRIANGLE_ANIM_H
#define TRIANGLE_ANIM_H

#include "Animator.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <tuple>

// Sloppily defined animating triangle for specific-use only.
// TODO: clean up, inherit from Triangle AND animator
class TriangleAnim : public Animator //, public Object
{
public:
    // Midpoint of triangle
    Eigen::Vector3d mp;
    // Corners
    Eigen::Vector3d a, b, c;

    // Constructor builds the triangle from a center point and inserts it into a bounding box
    TriangleAnim(Eigen::Vector3d &center, const int &level_height, const int &unit_size)
    {
        Eigen::Vector3d u(1, 0, 0);
        Eigen::Vector3d v(0, 1, 0);
        Eigen::Vector3d w(0, 0, 1);
        this->a = center - 0.25 * level_height * v - 0.25 * unit_size * u;
        this->b = center - 0.25 * level_height * v + 0.25 * unit_size * u;
        this->c = center + 0.25 * level_height * v;

        this->box.max_corner = center + 0.25 * level_height * v + 0.25 * unit_size * u + 0.25 * unit_size * w;
        this->box.min_corner = center - 0.25 * level_height * v - 0.25 * unit_size * u - 0.25 * unit_size * w;

        this->mp = center;
    }

    /*
    Given ray and min_t minimum parametric distance to consider,
    change t to parametric distance to intersection, n to normal on
    animated triangle at point of intersection.
    Return true if there is an intersection, false if not.
    */
    bool intersect(
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant) const;

    /*
    Animate this triangle! Rotate it on the spot.
    */
    void animate()
    {
        double angle = M_PI / 16.0;
        Eigen::Affine3d rotator = Eigen::Translation3d(this->mp) * Eigen::AngleAxisd(angle, Eigen::Vector3d(0, 1, 0)) * Eigen::Translation3d(-this->mp);
        ;
        this->a = rotator * a;
        this->b = rotator * b;
        this->c = rotator * c;
    }
};

#endif
