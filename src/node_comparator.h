#ifndef NODE_COMPARATOR_H
#define NODE_COMPARATOR_H

#include "node.h"

class NodePosComparator {
public:
    bool operator()(const Node &first, const Node &second) const;
};

class NodeFComparator {
public:
    bool operator()(const Node &first, const Node &second) const;
};

#endif  // NODE_COMPARATOR_H
