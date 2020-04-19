#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include <Eigen/Core>
#include <SDL.h>

/*
Handle the raytrace loop with a pixel resolution of width x height,
and fill the renderer with the corresponding colours to be pushed to screen.
*/
void raytrace(
    const int height,
    const int width,
    const Camera &camera,
    const std::shared_ptr<AABBTree> &root,
    const std::vector<std::shared_ptr<Light>> &lights,
    SDL_Renderer *&renderer);