#include "box_intersect_box.h"
bool box_intersect_box(
  const BoundingBox & A,
  const BoundingBox & B)
{
  double A_min_x = A.min_corner(0, 0);
  double A_min_y = A.min_corner(0, 1);
  double A_min_z = A.min_corner(0, 2);
  double A_max_x = A.max_corner(0, 0);
  double A_max_y = A.max_corner(0, 1);
  double A_max_z = A.max_corner(0, 2);

  double B_min_x = B.min_corner(0, 0);
  double B_min_y = B.min_corner(0, 1);
  double B_min_z = B.min_corner(0, 2);
  double B_max_x = B.max_corner(0, 0);
  double B_max_y = B.max_corner(0, 1);
  double B_max_z = B.max_corner(0, 2);

  // Check the conditions for non-intersection.
  if (A_max_x < B_min_x || B_max_x < A_min_x || 
      A_max_y < B_min_y || B_max_y < A_min_y || 
      A_max_z < B_min_z || B_max_z < A_min_z) {
        return false;
      }
  return true;

}
