#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Geometry>
#include "box_intersect_box.h"

bool Triangle::collide(BoundingBox &external_box)
{
    return box_intersect_box(external_box, this->box);
}

bool Triangle::intersect(
    const bool &is_from_viewer,
    const Ray &ray,
    const double min_t,
    double &t,
    Eigen::Vector3d &n,
    std::shared_ptr<Object> &descendant)
{
    // Collect and name vertices of triangle in counter-clockwise winding
    Eigen::Vector3d v0 = this->a;
    Eigen::Vector3d v1 = this->b;
    Eigen::Vector3d v2 = this->c;

    // Construct vectors representing barycentric axes of triangle, and compute normal
    Eigen::Vector3d t_a = v1 - v0;
    Eigen::Vector3d t_b = v2 - v0;

    // Now we want to solve Ax = B with x = [alpha, beta, t].
    // Create the matrices A and B.
    Eigen::Vector3d direction_neg = -1 * ray.direction;
    Eigen::Matrix<double, 3, 3> A;
    A << t_a[0], t_b[0], direction_neg[0],
        t_a[1], t_b[1], direction_neg[1],
        t_a[2], t_b[2], direction_neg[2];
    Eigen::Vector3d B = ray.origin - v0;

    // Solve for x.
    Eigen::Vector3d x = A.colPivHouseholderQr().solve(B);
    double alpha = x[0];
    double beta = x[1];
    double t_temp = x[2];

    // Check constraints and return.
    if (alpha >= 0 && beta >= 0 && alpha + beta <= 1 && t_temp >= min_t)
    {
        t = t_temp;
        n = t_a.cross(t_b);
        n.normalize();
        return true;
    }

    return false;
}