#include "SphereAnim.h"
#include "Ray.h"
#include <cmath>
#include "ray_intersect_box.h"

// TODO: remove redundant code, make this class inherit from Sphere and use its existing intersection method
bool SphereAnim::intersect(
    const Ray &ray, const double min_t, double &t, Eigen::Vector3d &n, std::shared_ptr<Object> &descendant) const
{
    // Set up quadratic equation for implicit sphere equation.
    Eigen::Vector3d e_minus_c = ray.origin - this->center;
    double a = ray.direction.dot(ray.direction);
    double b = 2 * (ray.direction.dot(e_minus_c));
    double c = (e_minus_c.dot(e_minus_c)) - (this->radius * this->radius);

    // Check discriminant for real roots.
    double discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0)
        return false;

    // Solve for roots, pick closest. If closer than min_t (image plane), discard.
    double soln_1 = (-b - sqrt(discriminant)) / (2 * a);
    double soln_2 = (-b + sqrt(discriminant)) / (2 * a);

    if (min_t <= soln_1 && (soln_2 < min_t || soln_1 < soln_2))
        t = soln_1;
    else if (min_t <= soln_2)
        t = soln_2;
    else              // soln_1 < min_t && soln_2 < min_t
        return false; // All hits are behind the image plane, so ignore them.

    // Calculate normal at intersection point, normalize.
    n = ((ray.origin + t * ray.direction) - this->center).normalized();

    // Successful intersection!
    return true;
}
