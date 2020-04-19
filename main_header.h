// Ray tracing
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "read_level.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "raytrace.h"
#include <cmath>
#include <Eigen/Core>

// AABBs and AABBTrees
#include "AABBTree.h"

// General use
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>

// SDL
#include <SDL.h>

// Helper function forward implementations

// Pixel/raycast resolution profiles 1-5
// TODO: Allow more resolution gradations on the lower end, kill the high end
void set_logical_resolution(SDL_Renderer*& renderer, int& w, int& h, const char& c)
{
    switch (c)
    {
    case '1': w = 80;   h = 45;  break;
    case '2': w = 160;  h = 90;  break;
    case '3': w = 320;  h = 180; break;
    case '4': w = 640;  h = 360; break;
    case '5': w = 1280; h = 720; break;
    default:  w = 160;  h = 90;
    }
    SDL_RenderSetLogicalSize(renderer, w, h);
}

void collect_animators(std::vector<std::shared_ptr<Object>>& objects,
    std::vector<std::shared_ptr<Animator>>& animators)
{
    animators.clear();
    animators.shrink_to_fit();
    std::shared_ptr<Animator> object_is_animator;
    for (int i = 0; i < objects.size(); i++)
    {
        object_is_animator = std::dynamic_pointer_cast<Animator>(objects[i]);
        if (object_is_animator)
            animators.push_back(object_is_animator);
    }
}

bool read_and_reset_level(int& argc,
    char* argv[],
    Camera& camera,
    std::vector<std::shared_ptr<Object>>& objects,
    std::vector<std::shared_ptr<Light>>& lights,
    std::shared_ptr<AABBTree>& root,
    std::vector<std::shared_ptr<Animator>>& animators)
{
    if (!read_level(argc <= 1 ? "../levels/defaultlevel.txt" : argv[1], camera, objects, lights))
    {
        std::cerr << "***** ERROR *****\nMalformed level! Closing...\n";
        return false;
    }
    // Build the bounding box tree from the objects provided.
    root = std::make_shared<AABBTree>(objects);

    // Collect the animators into a separate structure
    collect_animators(objects, animators);
    return true;
}

void animate_animators(std::vector<std::shared_ptr<Animator>>& animators)
{
    for (int i = 0; i < animators.size(); i++)
    {
        animators[i]->animate();
    }
}

void change_pitch(Camera& camera, const double& angle)
{
    Eigen::Affine3d Y = Eigen::Translation3d(Eigen::Vector3d(0, 0, 0)) *
        Eigen::AngleAxisd(angle, camera.u);
    camera.v = (Y * camera.v).normalized();
    camera.w = (Y * camera.w).normalized();
}

void change_roll(Camera& camera, const double& angle)
{
    Eigen::Affine3d R = Eigen::Translation3d(Eigen::Vector3d(0, 0, 0)) *
        Eigen::AngleAxisd(angle, camera.w);
    camera.u = (R * camera.u).normalized();
    camera.v = (R * camera.v).normalized();
}

void change_yaw(Camera& camera, const double& angle)
{
    Eigen::Affine3d Y = Eigen::Translation3d(Eigen::Vector3d(0, 0, 0)) *
        Eigen::AngleAxisd(angle, camera.v);
    camera.u = (Y * camera.u).normalized();
    camera.w = (Y * camera.w).normalized();
}