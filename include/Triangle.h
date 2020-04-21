#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include <Eigen/Core>
#include "insert_triangle_into_box.h"
#include <tuple>

class Triangle : public Object
{
public:
    // Three corners of the triangle.
    Eigen::Vector3d a, b, c;

    /*
    Constructor takes a tuple of corners and inserts the triangle into its
    own bounding box.
    */
    Triangle(
        const std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> &corners)
    {
        this->a = std::get<0>(corners);
        this->b = std::get<1>(corners);
        this->c = std::get<2>(corners);
        insert_triangle_into_box(this->a, this->b, this->c, this->box);
    }

    // Default constructor (should be useful for subclasses only!)
    Triangle() = default;

    /*
    Given ray and min_t minimum parametric distance to consider,
    change t to parametric distance to intersection, n to normal on
    the triangle at point of intersection.
    Return true if there is an intersection, false if not.
    */
    bool intersect(
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant) const;
};

#endif
