#include "insert_quad_into_box.h"

void insert_quad_into_box(
  const Eigen::Vector3d & a,
  const Eigen::Vector3d & b,
  const Eigen::Vector3d & c,
  BoundingBox & B)
{
    using namespace std;
    // Construct the fourth point from the 3 stored points.
    Eigen::Vector3d d = b + c - a;

    // Get the max values on each axis for this triangle.
    double x_max = max(max(a[0], b[0]), max(d[0], c[0]));
    double y_max = max(max(a[1], b[1]), max(d[1], c[1]));
    double z_max = max(max(a[2], b[2]), max(d[2], c[2]));

    // Get the min values on each axis for this triangle.
    double x_min = min(min(a[0], b[0]), min(d[0], c[0]));
    double y_min = min(min(a[1], b[1]), min(d[1], c[1]));
    double z_min = min(min(a[2], b[2]), min(d[2], c[2]));

    // Grow the max corner of the box as needed to contain this triangle.
    if (x_max > B.max_corner[0]) B.max_corner[0] = x_max;
    if (y_max > B.max_corner[1]) B.max_corner[1] = y_max;
    if (z_max > B.max_corner[2]) B.max_corner[2] = z_max;

    // Grow the min corner of the box as needed to contain this triangle.
    if (x_min < B.min_corner[0]) B.min_corner[0] = x_min;
    if (y_min < B.min_corner[1]) B.min_corner[1] = y_min;
    if (z_min < B.min_corner[2]) B.min_corner[2] = z_min;
}
