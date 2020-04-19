#ifndef MATERIAL_H
#define MATERIAL_H
#include <Eigen/Core>

// Blinn-Phong shading material parameters
struct Material
{
    // Ambient, diffuse, specular, mirror. Colour vectors
    Eigen::Vector3d ka, kd, ks, km;
    // Phong exponent, tau term for optimizing phong shading
    double phong_exponent, tau;
    // Flag for fullbright
    bool is_light = false;
};
#endif
