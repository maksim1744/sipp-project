#include "search.h"
#include <iostream>

void Search::generate_safe_intervals(int x, int y) {
    if (safe_intervals.find(std::make_pair(x, y)) != safe_intervals.end()) return;

    std::vector<int> busy_moments;
    int someone_stays_here = -1;
    for (auto &obstacle : *obstacles) {
        for (int i = 0; i < obstacle.path.size(); ++i) {
            if (obstacle.path[i] == std::make_pair(x, y)) {
                busy_moments.push_back(i);
                if (i + 1 == obstacle.path.size()) {
                    if (someone_stays_here == -1 || someone_stays_here > i)
                        someone_stays_here = i;
                }
            }
        }
    }
    std::sort(busy_moments.begin(), busy_moments.end());
    busy_moments.erase(unique(busy_moments.begin(), busy_moments.end()), busy_moments.end());
    busy_moments.insert(busy_moments.begin(), -1);



    std::vector<std::pair<int, int>> safe_intervals;

    for (int i = 0; i + 1 < busy_moments.size(); ++i) {
        if (busy_moments[i] + 1 < busy_moments[i + 1]) {
            safe_intervals.emplace_back(busy_moments[i] + 1, busy_moments[i + 1] - 1);
        }
    }
    if (someone_stays_here != -1) {
        while (!safe_intervals.empty() && safe_intervals.back().first >= someone_stays_here)
            safe_intervals.pop_back();
        if (!safe_intervals.empty()) {
            safe_intervals.back().second = std::min(safe_intervals.back().second, someone_stays_here - 1);
        }
    } else {
        safe_intervals.emplace_back(busy_moments.back() + 1, inf);
    }
    this->safe_intervals[std::make_pair(x, y)] = safe_intervals;

    std::vector<std::vector<std::pair<int, int>>> directions(safe_intervals.size());
    for (auto &obstacle : *obstacles) {
        int ind = -1;
        for (auto [l, r] : safe_intervals) {
            ++ind;
            if (l <= 0) continue;
            if (l >= obstacle.path.size()) break;
            if (obstacle.path[l - 1] == std::make_pair(x, y))
                directions[ind].push_back(obstacle.path[l]);
        }
    }
    for (int i = 0; i < directions.size(); ++i) {
        std::sort(directions[i].begin(), directions[i].end());
        directions[i].erase(unique(directions[i].begin(), directions[i].end()), directions[i].end());
    }
    this->directions[std::make_pair(x, y)] = directions;
}

std::vector<Node> Search::get_successors(const Node &node) {
    std::vector<std::pair<int, int>> dirs = {
        {-1, 0},
        { 1, 0},
        {0, -1},
        {0,  1}
    };

    std::vector<Node> result;

    int start_time = node.g;
    auto &node_safe_intervals = safe_intervals[node.position()];
    auto it_end = std::lower_bound(node_safe_intervals.begin(), node_safe_intervals.end(), std::make_pair(start_time, -1));
    if (it_end == node_safe_intervals.end() || it_end->first > start_time)
        --it_end;
    int end_time = it_end->second;
    // if (node.x == 5 && node.y == 10 && node.segment == 1 && false) {
    //     std::cout << "time: " << start_time << ' ' << end_time << std::endl;
    // }

    for (auto [dx, dy] : dirs) {
        int x1 = node.x + dx, y1 = node.y + dy;

        if (map->is_wall(x1, y1)) continue;

        // if (node.x == 5 && node.y == 10 && node.segment == 1 && false) {
        //     std::cout << "-----------------------" << std::endl;
        //     std::cout << x1 << ' ' << y1 << std::endl;
        // }

        generate_safe_intervals(x1, y1);

        auto &safe_intervals = this->safe_intervals[std::make_pair(x1, y1)];
        auto &directions = this->directions[std::make_pair(x1, y1)];

        int ind = lower_bound(safe_intervals.begin(), safe_intervals.end(), std::make_pair(start_time + 1, -1)) - safe_intervals.begin();
        if (ind != 0 && safe_intervals[ind - 1].second >= start_time + 1)
            --ind;
        // if (node.x == 5 && node.y == 10 && node.segment == 1 && false) {
        //     for (auto [l, r] : safe_intervals) {
        //         std::cout << "[" << l << ", " << r << "] ";
        //     }
        //     std::cout << std::endl;
        //     for (int i = 0; i < directions.size(); ++i) {
        //         std::cout << "[ ";
        //         for (auto [x, y] : directions[i]) {
        //             std::cout << "(" << x << "," << y << ") ";
        //         }
        //         std::cout << "]  ";
        //     }
        //     std::cout << std::endl;
        //     std::cout << "ind = " << ind << std::endl;
        // }
        for (; ind < safe_intervals.size() && safe_intervals[ind].first <= end_time + 1; ++ind) {
            bool can_go = true;
            for (auto [xto, yto] : directions[ind]) {
                if (xto == node.x && yto == node.y && safe_intervals[ind].first >= start_time) {
                    can_go = false;
                    break;
                }
            }
            if (!can_go) continue;

            Node successor;
            successor.x = x1;
            successor.y = y1;
            successor.segment = ind;
            successor.g = std::max(start_time + 1, safe_intervals[ind].first);

            result.push_back(successor);
        }
    }
    return result;
}

void Search::start_search(const Map &map, const std::vector<Obstacle> &obstacles, const Options &options, SearchResult *search_result) {
    this->map = &map;
    this->obstacles = &obstacles;
    this->options = &options;

    search_result->path_found = false;

    std::set<Node, NodeFComparator> open;
    std::set<Node, NodePosComparator> close;

    Node start_node;
    start_node.x = map.startx;
    start_node.y = map.starty;
    start_node.segment = 0;

    generate_safe_intervals(start_node.x, start_node.y);

    open.insert(start_node);

    Node *finish_node = nullptr;

    int cnt = 0;

    while (!open.empty()) {
        ++cnt;
        // std::cout << "open size: " << open.size() << std::endl;
        Node node = *open.begin();
        // std::cout << node.x << ' ' << node.y << ' ' << node.segment << std::endl;
        open.erase(open.begin());
        if (close.find(node) != close.end())
            continue;
        close.insert(node);
        if (node.x == map.finishx && node.y == map.finishy) {
            search_result->path_found = true;
            finish_node = (Node*)&(*close.find(node));
            break;
        }
        auto successors = get_successors(node);
        // std::cout << "successors size: " << successors.size() << std::endl;
        for (auto successor : successors) {
            if (close.find(successor) != close.end())
                continue;
            successor.parent = (Node*)&(*close.find(node));
            successor.h = 0; // TODO
            successor.f = successor.g + successor.h * 1;

            // std::cout << "{" << successor.x << ", " << successor.y << ", " << successor.segment << "}" << std::endl;

            open.insert(successor);
        }
        // break;
    }

    // std::cout << "cnt: " << cnt << std::endl;

    if (!search_result->path_found)
        return;

    std::vector<Node> path_preview;
    while (finish_node != nullptr) {
        path_preview.push_back(*finish_node);
        finish_node = finish_node->parent;
    }
    reverse(path_preview.begin(), path_preview.end());

    search_result->path.clear();
    for (int i = 0; i < path_preview.size(); ++i) {
        PathNode path_node;
        path_node.x = path_preview[i].x;
        path_node.y = path_preview[i].y;
        if (i + 1 < path_preview.size())
            path_node.time = path_preview[i + 1].g - path_preview[i].g - 1;
        else
            path_node.time = 0;

        search_result->path.push_back(path_node);
    }
}
