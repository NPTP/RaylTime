#include "AABBTree.h"
#include "box_intersect_box.h"

bool AABBTree::collide(
    BoundingBox &external_box)
{
    // If the external box does not even intersect this node, skip the node.
    if (!box_intersect_box(external_box, this->box))
        return false;

    // Set up for recursive intersection tests.
    double left_t, right_t;
    bool left_collide = false;
    bool right_collide = false;

    // Run through all checks of intersection.
    // Boolean check is short-circuited so null-checking this way is fine.
    if (this->left && this->left->collide(external_box))
        return true;
    if (this->right && this->right->collide(external_box))
        return true;

    // At this point, the external box intersects this node of the tree,
    // but not any object inside it, so return false.
    return false;
}
