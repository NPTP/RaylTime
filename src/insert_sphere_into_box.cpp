#include "insert_sphere_into_box.h"

void insert_sphere_into_box(
    const Eigen::Vector3d &center,
    const double &radius,
    BoundingBox &B)
{
    // Get the max values on each axis for this sphere.
    double x_max = center[0] + radius;
    double y_max = center[1] + radius;
    double z_max = center[2] + radius;

    // Get the min values on each axis for this sphere.
    double x_min = center[0] - radius;
    double y_min = center[1] - radius;
    double z_min = center[2] - radius;

    // Grow the max corner of the box as needed to contain this sphere.
    if (x_max > B.max_corner[0]) { B.max_corner[0] = x_max; }
    if (y_max > B.max_corner[1]) { B.max_corner[1] = y_max; }
    if (z_max > B.max_corner[2]) { B.max_corner[2] = z_max; }

    // Grow the min corner of the box as needed to contain this sphere.
    if (x_min < B.min_corner[0]) { B.min_corner[0] = x_min; }
    if (y_min < B.min_corner[1]) { B.min_corner[1] = y_min; }
    if (z_min < B.min_corner[2]) { B.min_corner[2] = z_min; }
}
