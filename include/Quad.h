#ifndef QUAD_H
#define QUAD_H

#include "Object.h"
#include <Eigen/Core>
#include "insert_quad_into_box.h"
#include <tuple>

// Actually a parallelogram defined by 3 points, not strictly an "any four points" quad.
class Quad : public Object
{
public:
    // The 3 corners that define the parallelogram (4th is computed)
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> corners;

    // Constructor inserts the rectangle into its own bounding box.
    Quad(
        const std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> &corners)
    {
        insert_quad_into_box(
            std::get<0>(corners),
            std::get<1>(corners),
            std::get<2>(corners),
            this->box);
        this->corners = corners;
    }

    /*
    Given ray and min_t minimum parametric distance to consider,
    change t to parametric distance to intersection, n to normal on quad
    at point of intersection.
    Return true if there is an intersection, false if not.
    */
    bool intersect(
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant);
};

#endif
