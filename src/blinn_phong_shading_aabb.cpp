#include "blinn_phong_shading_aabb.h"
#include <cmath>

// Ambient light default level and reflection displacement constant
#define AMBIENT_LIGHT_DEFAULT_VAL 0.1
#define BP_EPSILON 1e-6

Eigen::Vector3d blinn_phong_shading_aabb(
    const Ray &ray,
    const std::shared_ptr<Object> &descendant,
    const double &t,
    const Eigen::Vector3d &n,
    const std::shared_ptr<AABBTree> &root,
    const std::vector<std::shared_ptr<Light>> &lights)
{
    // Ambient term. Initialize blinn_phong rgb vector.
    // Set to fullbright (1.0) when this is a light or a bounding box.
    double ambient_ia;
    if (descendant->material->is_light)
        ambient_ia = 1.0;
    else
        ambient_ia = AMBIENT_LIGHT_DEFAULT_VAL;
    Eigen::Vector3d blinn_phong = descendant->material->ka * ambient_ia;

    Ray ray_to_light;
    ray_to_light.origin = ray.origin + t * ray.direction;
    double t_to_light, t_to_object;
    Eigen::Vector3d l, dummy_normal;
    std::shared_ptr<Object> dummy_descendant;

    // Iterate through lights for this object.
    for (int i = 0; i < lights.size(); i++)
    {
        // Put the direction toward the light in the (normalized) vector l.
        lights[i]->direction(ray_to_light.origin, l, t_to_light);

        // If the ray-to-light origin is outside of the light's radius, skip this light.
        if (t_to_light > lights[i]->light_radius)
            continue;

        // Give ray to light the direction to THIS light.
        ray_to_light.direction = l;

        // First argument to intersect() tells the ray it is NOT direct from the viewer (for draw distance).
        bool hit_object = root->intersect(false, ray_to_light, BP_EPSILON, t_to_object, dummy_normal, dummy_descendant);

        // Check if there was no hit, meaning no blocked path to the light.
        // Also check if there was a hit, but that hit was further away than the light.
        // In either case the light is able to reach this point.
        if (!hit_object || (hit_object && t_to_light < t_to_object))
        {
            // Set Lambertian diffuse rgb vector.
            Eigen::Vector3d lambertian = (descendant->material->kd.cwiseProduct(lights[i]->I)) * std::max(0.0, n.dot(l));

            // Construct h, the half-vector, and set specular rgb vector.
            Eigen::Vector3d v, h, specular;
            v = (-1 * ray.direction).normalized();
            h = (v + l).normalized();
            double n_dot_h = n.dot(h);
            if (n_dot_h <= descendant->material->tau) // If specular term is negligible, skip computation
                specular = Eigen::Vector3d(0.0, 0.0, 0.0);
            else
                specular = (descendant->material->ks.cwiseProduct(lights[i]->I)) * pow(std::max(0.0, n_dot_h), descendant->material->phong_exponent);

            // Adjust for distance from light radius and sum Blinn-Phong rgb values.
            double light_scale = (-t_to_light / lights[i]->light_radius) + 1;
            Eigen::Vector3d light_addition = (lambertian + specular) * light_scale;
            blinn_phong = blinn_phong + light_addition;
        }
    }

    return blinn_phong;
}
