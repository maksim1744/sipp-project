#include "mission.h"

bool Mission::run_search() {
    search.start_search(map, obstacles, options, &search_result);
    return true;
}
