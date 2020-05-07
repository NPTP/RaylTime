#ifndef SHOTLIGHT_H
#define SHOTLIGHT_H
#include "PointLight.h"
#include "Animator.h"
#include "BoundingBox.h"
#include <Eigen/Core>
#define MOVE_LIGHT_RADIUS 5  // Set radius of light to taste
#define MOVE_LIGHT_STEP 0.25 // Set step size of light/speed of the light to taste. Keep as a clean divisor of 1.

class MovingLight : public PointLight, public Animator
{
public:
    // North/South or East/West, i.e., -w/+w or +u/-u
    Eigen::Vector3d orientation;
    // Maximum number of steps the light can take in one direction
    // before doubling back. Keep track with counter.
    int max_steps;
    int counter;

    MovingLight(Eigen::Vector3d orientation, double light_radius, int tile_size)
    {
        this->orientation = orientation.normalized();
        this->light_radius = light_radius;
        this->max_steps = 2 * (tile_size / MOVE_LIGHT_STEP); // Light will move 2 tiles in each direction
        this->counter = 0;
    }

    void animate()
    {
        if (counter < max_steps)
        {
            this->p += this->orientation * MOVE_LIGHT_STEP;
            counter++;
        }
        else if (counter == 4 * max_steps)
        {
            this->p += this->orientation * MOVE_LIGHT_STEP;
            counter = 1;
        }
        else if (counter >= 3 * max_steps)
        {
            this->p += this->orientation * MOVE_LIGHT_STEP;
            counter++;
        }
        else if (counter >= max_steps)
        {
            this->p -= this->orientation * MOVE_LIGHT_STEP;
            counter++;
        }
    }
};
#endif
