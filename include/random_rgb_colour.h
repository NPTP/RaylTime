#ifndef RANDOM_RGB_COLOUR_H
#define RANDOM_RGB_COLOUR_H

#include <random>
#include <Eigen/Core>

std::uniform_real_distribution<double> urd(0.0, 1.0);
std::default_random_engine dre;

/*
Global random colour generator (globalized to prevent repeated colours as
AABB tree is constructed. Returns a 3-vector of random RGB values in [0.0, 1.0].
*/
Eigen::Vector3d random_rgb_colour()
{
    return Eigen::Vector3d(urd(dre), urd(dre), urd(dre));
}

#endif