#include <Eigen/Core>

Eigen::Vector3d reflect(const Eigen::Vector3d & in, const Eigen::Vector3d & n)
{
  // Calculate reflected ray direction and normalize.
  Eigen::Vector3d r = in + (-1 * (2 * in.dot(n) * n));
  return r.normalized();
}
