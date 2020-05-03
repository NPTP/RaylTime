#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include <Eigen/Core>

/*
The source of the user's vision, with its own position, u/v/w basis,
focal length, and image plane dimensions.
*/
struct Camera
{
    // Position
    Eigen::Vector3d e;

    // Orthonormal frame so that -w is the viewing direction.
    Eigen::Vector3d u, v, w;

    // Image plane distance / focal length
    double d;

    // Width and height of image plane
    double width, height;

    // Collision box for camera
    BoundingBox box;
};

#endif
