#ifndef OBJECT_H
#define OBJECT_H

#include "BoundingBox.h"
#include "Material.h"
#include <Eigen/Core>
#include <memory>

struct Ray;
class Object
{
public:
    // This Object's material for shading.
    std::shared_ptr<Material> material;

    virtual ~Object() {}

    /*
    Given ray and min_t minimum parametric distance to consider,
    change t to parametric distance to intersection, n to normal on object
    at point of intersection.
    Return true if there is an intersection, false if not.
    */
    virtual bool intersect(
        const Ray &ray,
        const double min_t,
        double &t,
        Eigen::Vector3d &n,
        std::shared_ptr<Object> &descendant) = 0; // = 0 ensures function is defined (as no-op)

    BoundingBox box;
};

#endif
