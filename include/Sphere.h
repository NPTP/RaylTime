#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "insert_sphere_into_box.h"
#include <Eigen/Core>

class Sphere : public Object
{
public:
    Eigen::Vector3d center;
    double radius;

    // Constructor inserts the sphere into its own bounding box.
    Sphere(const Eigen::Vector3d center,
           const double radius)
    {
        insert_sphere_into_box(
            center,
            radius,
            this->box);
        this->center = center;
        this->radius = radius;
    }

    // Default constructor (should be useful for subclasses only!)
    Sphere() = default;

    /*
    Given ray and min_t minimum parametric distance to consider,
    change t to parametric distance to intersection, n to normal on sphere
    at point of intersection.
    Return true if there is an intersection, false if not.
    */
    bool intersect(
        const bool &is_from_viewer,
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant);
};

#endif
