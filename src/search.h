#ifndef SEARCH_H
#define SEARCH_H

#include "map.h"
#include "obstacle.h"
#include "options.h"
#include "path_node.h"
#include "search_result.h"

#include <map>
#include <set>
#include <utility>
#include <vector>

class Search {
public:
    void start_search(const Map &map, const std::vector<Obstacle> &obstacles, const Options &options, SearchResult *search_result);
};

#endif  // SEARCH_H
