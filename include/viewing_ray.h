#ifndef VIEWING_RAY_H
#define VIEWING_RAY_H

#include "Ray.h"
#include "Camera.h"

/* 
Construct a viewing ray into 'ray', given a camera, pixel location (i = col, j = row)
and number of pixels width/height of image, such that ray.origin. + t*ray.direction
lands exactly on the center of the pixel (i,j).
*/
void viewing_ray(
    const Camera &camera,
    const int i,
    const int j,
    const int width,
    const int height,
    Ray &ray);
#endif
