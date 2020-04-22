#ifndef MISSION_H
#define	MISSION_H

#include "map.h"
#include "obstacle.h"
#include "options.h"
#include "search.h"
#include "search_result.h"

#include <vector>

class Mission {
public:
    Mission() {};

    bool run_search();

    Map map;
    std::vector<Obstacle> obstacles;
    Search search;
    SearchResult search_result;
    Options options;
};

#endif  // MISSION_H
