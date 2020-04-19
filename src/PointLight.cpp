#include "PointLight.h"

void PointLight::direction(
    const Eigen::Vector3d &q, Eigen::Vector3d &d, double &max_t) const
{
    // Get direction from query point to pointlight
    Eigen::Vector3d d_temp = this->p - q;

    // Calculate magnitude of this new direction vector for parametric distance max_t
    max_t = d_temp.norm();

    // Store normalized direction vector in d
    d = d_temp.normalized();
}
