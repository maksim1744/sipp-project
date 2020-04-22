#include "map.h"

bool Map::is_wall(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height)
        return true;
    return walls[y][x];
}
