#ifndef MAP_H
#define MAP_H

#include <vector>

class Map {
public:
    Map() {}

    bool is_wall(int x, int y) const;

    int width;
    int height;
    int startx;
    int starty;
    int finishx;
    int finishy;

    std::vector<std::vector<bool>> walls;
};

#endif  // MAP_H
