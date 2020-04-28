#include "raycolor.h"
#include "blinn_phong_shading_aabb.h"
#include "reflect.h"
#include "globals.h"

#define REFLECT_EPSILON 1e-6

bool raycolor(
    const bool &is_from_viewer,
    const Ray &ray,
    const double min_t,
    const std::shared_ptr<AABBTree> &root,
    const std::vector<std::shared_ptr<Light>> &lights,
    const int num_recursive_calls,
    Eigen::Vector3d &rgb)
{
    std::shared_ptr<Object> descendant; // Pointer to object with first hit
    double t_hit;                       // Parametric distance along ray (ray.origin + t_hit*ray.direction is  hit location)
    Eigen::Vector3d n;                  // Surface normal at hit location

    // Find first hit.
    // Surface normal "n" gets normalized inside the collision functions for each primitive.
    bool hit = root->intersect(is_from_viewer, ray, min_t, t_hit, n, descendant);

    if (hit && t_hit < G_max_t_draw_distance)
    {
        // Calculate Blinn-Phong shading for this hit and add to rolling rgb totals.
        Eigen::Vector3d blinn_phong_rgb = blinn_phong_shading_aabb(ray, descendant, t_hit, n, root, lights);

        rgb = rgb + blinn_phong_rgb;

        // Do not reflect the ray if (1) the reflectivity of the material is (0,0,0) -AND- (2) call is outisde recursive bounds.
        if (descendant->material->km != Eigen::Vector3d(0.0, 0.0, 0.0) && num_recursive_calls < G_raytrace_recursion_depth)
        {
            // Get new ray direction
            Ray reflected_ray;
            reflected_ray.origin = ray.origin + t_hit * ray.direction;
            reflected_ray.direction = reflect(ray.direction.normalized(), n);

            Eigen::Vector3d rgb_temp(0, 0, 0);

            // Send out the new (reflected) ray and collect color information.
            // First argument will increase performance as is, but arguably this ray is NOT direct from the viewer.
            raycolor(is_from_viewer, reflected_ray, REFLECT_EPSILON, root, lights, num_recursive_calls + 1, rgb_temp);

            rgb = rgb + descendant->material->km.cwiseProduct(rgb_temp);
        }

        rgb = rgb * ((-t_hit / G_max_t_draw_distance) + 1);

        return true;
    }

    return false;
}
