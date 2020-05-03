#include "Quad.h"
#include "Ray.h"
#include "box_intersect_box.h"
#include <Eigen/Geometry>

bool Quad::collide(BoundingBox &external_box)
{
    return box_intersect_box(external_box, this->box);
}

bool Quad::intersect(
    const bool &is_from_viewer,
    const Ray &ray,
    const double min_t,
    double &t,
    Eigen::Vector3d &n,
    std::shared_ptr<Object> &descendant)
{
    // Collect and name vertices of parallelogram in counter-clockwise winding
    Eigen::Vector3d v0 = std::get<0>(this->corners);
    Eigen::Vector3d v1 = std::get<1>(this->corners);
    Eigen::Vector3d v2 = std::get<2>(this->corners);

    // Construct vectors representing barycentric axes of parallelogram, and compute normal
    Eigen::Vector3d w = v1 - v0;
    Eigen::Vector3d l = v2 - v0;

    // Now we want to solve Ax = B with x = [alpha, beta, t].
    // Create the matrices A and B.
    Eigen::Vector3d direction_neg = -1 * ray.direction;
    Eigen::Matrix<double, 3, 3> A;
    A << w[0], l[0], direction_neg[0],
        w[1], l[1], direction_neg[1],
        w[2], l[2], direction_neg[2];
    Eigen::Vector3d B = ray.origin - v0;

    // Solve for x.
    Eigen::Vector3d x = A.colPivHouseholderQr().solve(B);
    double alpha = x[0];
    double beta = x[1];
    double t_temp = x[2];

    // Check constraints and return.
    if (0 <= alpha && 0 <= beta && alpha <= 1 && beta <= 1 && t_temp >= min_t)
    {
        t = t_temp;
        n = w.cross(l).normalized();
        return true;
    }
    else
        return false;
}
