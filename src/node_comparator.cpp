#include "node_comparator.h"

bool NodePosComparator::operator()(const Node &first, const Node &second) const {
    if (first.x != second.x)
        return first.x < second.x;
    if (first.y != second.y)
        return first.y < second.y;
    if (first.segment != second.segment)
        return first.segment < second.segment;
    return false;
}

bool NodeFComparator::operator()(const Node &first, const Node &second) const {
    if (first.f != second.f)
        return first.f < second.f;
    if (first.g != second.g)
        return first.g > second.g;
    if (first.x != second.x)
        return first.x < second.x;
    if (first.y != second.y)
        return first.y < second.y;
    if (first.segment != second.segment)
        return first.segment < second.segment;
    return false;
}
