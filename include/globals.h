#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
// Global variables for technical settings

/* Flag using bit-flip to activate/deactivate AABB visualization. */
extern uint8_t G_show_boxes;

/* Tells us at which depth of the tree to visualize the AABBs.*/
extern int G_show_boxes_depth;

/* Gives the recursion depth of raytracing for reflections. */
extern int G_raytrace_recursion_depth;

#endif