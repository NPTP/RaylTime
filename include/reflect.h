#ifndef REFLECT_H
#define REFLECT_H
#include <Eigen/Core>

/*
Given incoming ray and unit normal at hit location, return
reflected outward ray.
*/
Eigen::Vector3d reflect(const Eigen::Vector3d &in, const Eigen::Vector3d &n);
#endif
