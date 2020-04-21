#ifndef TRIANGLE_ANIM_H
#define TRIANGLE_ANIM_H

#include "Triangle.h"
#include "Animator.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <tuple>

/* Animated Triangle with specific dimensions in constructor. */
class TriangleAnim : public Triangle, public Animator
{
public:
    // Midpoint of triangle
    Eigen::Vector3d mp;

    // Constructor builds the triangle from a center point and inserts it into a bounding box
    TriangleAnim(Eigen::Vector3d &center, const int &level_height, const int &unit_size)
    {
        Eigen::Vector3d u(1, 0, 0);
        Eigen::Vector3d v(0, 1, 0);
        Eigen::Vector3d w(0, 0, 1);
        this->a = center - 0.25 * level_height * v - 0.25 * unit_size * u;
        this->b = center - 0.25 * level_height * v + 0.25 * unit_size * u;
        this->c = center + 0.25 * level_height * v;
        
        // Put this triangle animator in its box
        this->box.max_corner = center + 0.25 * level_height * v + 0.25 * unit_size * u + 0.25 * unit_size * w;
        this->box.min_corner = center - 0.25 * level_height * v - 0.25 * unit_size * u - 0.25 * unit_size * w;

        this->mp = center;
    }

    /*
    Animate this triangle! Rotate it in place on the v-axis (0,1,0) by 'angle'.
    */
    void animate()
    {
        double angle = M_PI / 16.0;
        Eigen::Affine3d rotator = Eigen::Translation3d(this->mp) * Eigen::AngleAxisd(angle, Eigen::Vector3d(0, 1, 0)) * Eigen::Translation3d(-this->mp);
        this->a = rotator * a;
        this->b = rotator * b;
        this->c = rotator * c;
    }
};

#endif
