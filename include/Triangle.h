#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include <Eigen/Core>
#include "insert_triangle_into_box.h"
#include <tuple>

class Triangle : public Object
{
public:
    // Three corners of triangle.
    // TODO: reconsider the use of the tuple, it just adds extra ops. Maybe just have the 3 vectors as they are, like TriangleAnim.
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> corners;

    // Constructor inserts the triangle into its own bounding box.
    Triangle(
        const std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> &corners)
    {
        insert_triangle_into_box(
            std::get<0>(corners),
            std::get<1>(corners),
            std::get<2>(corners),
            this->box);
        this->corners = corners;
    }

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
