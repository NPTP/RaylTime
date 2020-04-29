#include "DirectionalLight.h"
#include <limits>

void DirectionalLight::direction(
    const Eigen::Vector3d &q, Eigen::Vector3d &d, double &t_to_light) const
{
    d = -(this->light_direction).normalized();
    t_to_light = std::numeric_limits<double>::infinity();
}
