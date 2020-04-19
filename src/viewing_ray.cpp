#include "viewing_ray.h"

void viewing_ray(
    const Camera &camera,
    const int i,
    const int j,
    const int width,  // num pixels in x direction (nx)
    const int height, // num pixels in y direction (ny)
    Ray &ray)
{
    // Set origin of ray - the camera "eye".
    ray.origin = camera.e;

    // Compute the scaling factors for u, v and w.
    double u_scale = ((j + 0.5) / width) * camera.width - camera.width * 0.5;
    double v_scale = (((height - i) - 0.5) / height) * camera.height - camera.height * 0.5;
    double w_scale = -camera.d;

    // Get ray direction in current basis.
    ray.direction = (camera.e +
                     camera.u * u_scale +
                     camera.v * v_scale +
                     camera.w * w_scale) -
                    ray.origin;
}
