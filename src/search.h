#ifndef SEARCH_H
#define SEARCH_H

#include "map.h"
#include "node.h"
#include "node_comparator.h"
#include "obstacle.h"
#include "options.h"
#include "path_node.h"
#include "search_result.h"

#include <algorithm>
#include <ctime>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

class Search {
public:
    Search() {}

    void start_search(const Map &map, const std::vector<Obstacle> &obstacles, const Options &options, SearchResult *search_result);

private:
    void calculate_dijkstra_heuristic();
    int get_heuristic(std::pair<int, int> p);
    void generate_safe_intervals(int x, int y);
    std::vector<Node> get_successors(const Node &node );

    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> safe_intervals;

    // this vector shows where went objects before each safe interval
    // for example, if we consider cell (0,0) and obstacle moved from (0,0) to (0,1)
    // at 4-th second, we can't move from (0,1) to (0,0) at the same time
    std::map<std::pair<int, int>, std::vector<std::vector<std::pair<int, int>>>> directions;
    std::vector<std::vector<int>> dijkstra_heuristic;

    const Map *map;
    const Options *options;
    const std::vector<Obstacle> *obstacles;

    const int inf = 1e9;
};

#endif  // SEARCH_H
