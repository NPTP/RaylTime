#include "ray_intersect_box.h"
#include <limits>

void component_t_min_max(double &t_c_min, double &t_c_max, double c_min, double c_max, double c_e, double c_d)
{
    if (c_d >= 0)
    {
        if (c_d == 0)
        {
            c_d = 0.0; // Remove any negative signs if c_d was -0
        }
        t_c_min = (c_min - c_e) / c_d;
        t_c_max = (c_max - c_e) / c_d;
    }
    else if (c_d < 0)
    {
        t_c_min = (c_max - c_e) / c_d;
        t_c_max = (c_min - c_e) / c_d;
    }
}

bool ray_intersect_box(
    const bool &is_from_viewer,
    const Ray &ray,
    const BoundingBox &box,
    const double min_t,
    const double max_t)
{
    // Get t min & max for x, y, z components.
    double t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;
    component_t_min_max(t_x_min, t_x_max, box.min_corner[0], box.max_corner[0], ray.origin[0], ray.direction[0]);
    component_t_min_max(t_y_min, t_y_max, box.min_corner[1], box.max_corner[1], ray.origin[1], ray.direction[1]);
    component_t_min_max(t_z_min, t_z_max, box.min_corner[2], box.max_corner[2], ray.origin[2], ray.direction[2]);

    double draw_dist;
    if (is_from_viewer)
        draw_dist = max_t;
    else
        draw_dist = std::numeric_limits<double>::infinity();

    // Check intervals. Include checks against min_t and max_t to ensure
    // there exists and overlap of intervals among all types of t's.
    if (t_x_min > t_y_max || t_y_min > t_x_max ||
        t_x_min > t_z_max || t_z_min > t_x_max ||
        t_x_min > draw_dist || min_t > t_x_max ||
        t_y_min > t_z_max || t_z_min > t_y_max ||
        t_y_min > draw_dist || min_t > t_y_max ||
        t_z_min > draw_dist || min_t > t_z_max)
    {
        return false;
    }

    return true;
}
