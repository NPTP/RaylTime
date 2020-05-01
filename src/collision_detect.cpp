#include "collision_detect.h"
#include "AABBTree.h"
#include "box_intersect_box.h"

bool collision_detect(
    const BoundingBox &viewbox,
    std::shared_ptr<AABBTree> &root)
{
    // TODO: implement some kind of collision detect thing in Objects which is based on
    // ray intersection, but checks box intersection, ending at a primitive

    // Recursively look through trees. AABB implementation just checks if box A intersects its own box,
    // then calls the check on left and right if so. If both return false, then it returns false overall.
    // If any return true, then it returns true instantly.
    // Object implementation checks if box A intersects its own box, returns true if so, false if not.
    if (false) // (root->collision())
        return true;

    return false;
}
