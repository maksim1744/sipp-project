#ifndef OPTIONS_H
#define OPTIONS_H

class Options {
public:
    const static int TYPE_MANHATTAN = 1;
    const static int TYPE_DIJKSTRA = 2;
    int heuristic_type;
};

#endif  // OPTIONS_H
