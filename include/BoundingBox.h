#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H
#include <Eigen/Core>

/*
Box containing properties for AABB tree construction and intersection tests.
*/
struct BoundingBox
{
    Eigen::RowVector3d min_corner;
    Eigen::RowVector3d max_corner;

    BoundingBox(
        Eigen::RowVector3d a_min_corner =
            Eigen::RowVector3d::Constant(1, 3, std::numeric_limits<double>::infinity()),
        Eigen::RowVector3d a_max_corner =
            Eigen::RowVector3d::Constant(1, 3, -std::numeric_limits<double>::infinity()))
        : min_corner(std::move(a_min_corner)),
          max_corner(std::move(a_max_corner))
    {
    }

    // TODO: (Leave until end) Do we use this function at all? Can we get rid of it?
    Eigen::RowVector3d center() const
    {
        return 0.5 * (max_corner + min_corner);
    }
};

#endif