#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include "path_node.h"

class SearchResult {
public:
    bool path_found = false;
    std::vector<PathNode> path;
};

#endif  // SEARCH_RESULT_H
