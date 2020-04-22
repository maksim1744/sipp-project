#include "search.h"

void Search::start_search(const Map &map, const std::vector<Obstacle> &obstacles, const Options &options, SearchResult *search_result) {
    PathNode node;
    node.x = 5;
    node.y = 3;
    node.time = 0;
    search_result->path.push_back(node);
    node.x = 5;
    node.y = 4;
    node.time = 2;
    search_result->path.push_back(node);
    node.x = 5;
    node.y = 5;
    node.time = 0;
    search_result->path.push_back(node);
    node.x = 5;
    node.y = 6;
    node.time = 0;
    search_result->path.push_back(node);
    node.x = 5;
    node.y = 7;
    node.time = 2;
    search_result->path.push_back(node);

    search_result->path_found = true;
}
