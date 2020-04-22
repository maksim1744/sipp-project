#ifndef NODE_H
#define NODE_H

#include <utility>

class Node {
public:
    int x, y;
    int segment;
    double f = 0, h = 0;
    int g = 0;

    Node *parent = nullptr;

    std::pair<int, int> position() const { return {x, y}; }
};

#endif  // NODE_H
