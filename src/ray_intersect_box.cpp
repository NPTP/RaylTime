#include "ray_intersect_box.h"

void get_3d_vector_components(double &x, double &y, double &z, const Eigen::Vector3d &v);
void get_3d_vector_components(double &x, double &y, double &z, const Eigen::RowVector3d &v);
void component_t_min_max(double &t_c_min, double &t_c_max, double c_min, double c_max, double c_e, double c_d);

bool ray_intersect_box(
    const Ray &ray,
    const BoundingBox &box,
    const double min_t,
    const double max_t)
{
    // TODO: clean this up, move helper func, get rid of forward decls above etc

    // Get components of ray origin (e).
    double x_e, y_e, z_e;
    get_3d_vector_components(x_e, y_e, z_e, ray.origin);

    // Get components of ray direction (d).
    double x_d, y_d, z_d;
    get_3d_vector_components(x_d, y_d, z_d, ray.direction);

    // Get min components of bounding box.
    double x_min, y_min, z_min;
    get_3d_vector_components(x_min, y_min, z_min, box.min_corner);

    // Get max components of bounding box.
    double x_max, y_max, z_max;
    get_3d_vector_components(x_max, y_max, z_max, box.max_corner);

    // Get t min & max for x, y, z components.
    double t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;
    component_t_min_max(t_x_min, t_x_max, x_min, x_max, x_e, x_d);
    component_t_min_max(t_y_min, t_y_max, y_min, y_max, y_e, y_d);
    component_t_min_max(t_z_min, t_z_max, z_min, z_max, z_e, z_d);

    // Check intervals. Include checks against min_t and max_t to ensure
    // there exists and overlap of intervals among all types of t's.
    if (t_x_min > t_y_max || t_y_min > t_x_max ||
        t_x_min > t_z_max || t_z_min > t_x_max ||
        t_x_min > max_t || min_t > t_x_max ||
        t_y_min > t_z_max || t_z_min > t_y_max ||
        t_y_min > max_t || min_t > t_y_max ||
        t_z_min > max_t || min_t > t_z_max)
    {
        return false;
    }

    return true;
}

// Helper functions for ray_intersect_box.
void get_3d_vector_components(double &x, double &y, double &z, const Eigen::Vector3d &v)
{
    x = v[0];
    y = v[1];
    z = v[2];
}

// Overload for RowVector.
void get_3d_vector_components(double &x, double &y, double &z, const Eigen::RowVector3d &v)
{
    x = v[0];
    y = v[1];
    z = v[2];
}

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