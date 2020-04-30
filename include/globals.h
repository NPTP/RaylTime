#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
// Global variables for technical settings

/* Height (i.e., max depth) of the AABB tree in the current scene. Used for AABB visualization. */
extern int G_aabb_tree_height;

/* Flag using bit-flip to activate/deactivate AABB visualization. */
extern uint8_t G_show_boxes;

/* Tells us at which depth of the tree to visualize the AABBs.*/
extern int G_show_boxes_depth;

/* Gives the recursion depth of raytracing for reflections. */
extern int G_raytrace_recursion_depth;

/* Draw distance in parametric value. */
extern int G_max_t_draw_distance;

#endif