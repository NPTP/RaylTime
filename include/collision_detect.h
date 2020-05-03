#ifndef COLLISION_DETECT_H
#define COLLISION_DETECT_H

#include "BoundingBox.h"
#include "AABBTree.h"

bool collision_detect(
    BoundingBox &viewbox,
    std::shared_ptr<AABBTree> &root);

#endif