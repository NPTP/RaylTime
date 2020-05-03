#include "collision_detect.h"
#include "AABBTree.h"
#include "box_intersect_box.h"

bool collision_detect(
    BoundingBox &viewbox,
    std::shared_ptr<AABBTree> &root)
{
    return root->collide(viewbox);
}
