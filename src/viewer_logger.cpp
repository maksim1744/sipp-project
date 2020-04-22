#include "viewer_logger.h"

void ViewerLogger::write_to_file(const char *filename, const Mission &mission) {
    std::ofstream fout;
    fout.open(filename);

    write_map(fout, mission.map);
    if (mission.search_result.path_found)
        write_objects(fout, mission);
    else
        fout << "tick\nmsg path not found\n";

    fout.close();
}

void ViewerLogger::write_map(std::ofstream &fout, const Map &map) {
    fout << "size (" << map.width << "," << map.height << ")\n";
    fout << "speed " << framerate * 2 << '\n';
    fout << "width 3" << '\n';
    fout << "flipy" << '\n';
    fout << "rect f=1 c=(" << (double)map.width / 2 << "," << (double)map.height / 2 << ") s=("
         << map.width + 20 << "," << map.height + 20 << ") col=(235,235,235)\n";
    for (int x = 0; x < map.width; ++x) {
        for (int y = 0; y < map.height; ++y) {
            if (map.is_wall(x, y)) {
                fout << "rect f=1 s=(1,1) col=(50,50,50) c=(" << 0.5 + x << "," << 0.5 + y << ")\n";
            }
        }
    }
    for (int x = 0; x <= map.width; ++x) {
        fout << "line s=(" << x << "," << 0 << ") f=(" << x << ", " << map.height << ") col=(200,200,200)\n";
    }
    for (int y = 0; y <= map.height; ++y) {
        fout << "line s=(" << 0 << "," << y << ") f=(" << map.width << ", " << y << ") col=(200,200,200)\n";
    }
    fout << "rect f=0 c=(" << (double)map.width / 2 << "," << (double)map.height / 2 << ") s=("
         << map.width << "," << map.height << ") col=(0,0,0)\n";

    fout << "circle f=1 col=(0,255,0) r=0.2 c=(" << 0.5 + map.startx << "," << 0.5 + map.starty << ")\n";
    fout << "circle f=1 col=(0,255,0) r=0.2 c=(" << 0.5 + map.finishx << "," << 0.5 + map.finishy << ")\n";
}

void ViewerLogger::write_objects(std::ofstream &fout, const Mission &mission) {
    std::vector<std::pair<int, int>> agent_path;
    for (auto node : mission.search_result.path) {
        for (int t = 0; t < node.time + 1; ++t) {
            agent_path.emplace_back(node.x, node.y);
        }
    }
    int total_steps = (int)agent_path.size() - 1;
    total_steps *= framerate;

    for (int i = 0; i + 1 < mission.search_result.path.size(); ++i) {
        auto from = mission.search_result.path[i];
        auto to = mission.search_result.path[i + 1];
        fout << "line col=(0,255,0) s=(" << 0.5 + from.x << "," << 0.5 + from.y << ") f=(" << 0.5 + to.x << "," << 0.5 + to.y << ")\n";
    }

    auto interpolate = [&](const std::vector<std::pair<int, int>> &path, int tm) {
        int ind = tm / framerate;
        double here = tm % framerate;
        if (ind + 1 >= path.size()) return std::make_pair((double)path.back().first, (double)path.back().second);
        double x = (double)path[ind].first + (double)(path[ind + 1].first - path[ind].first) * here / framerate;
        double y = (double)path[ind].second + (double)(path[ind + 1].second - path[ind].second) * here / framerate;
        return std::make_pair(x, y);
    };

    fout << std::setprecision(5) << std::fixed;
    for (int i = 0; i <= total_steps; ++i) {
        fout << "tick\n";
        fout << "msg second: " << i / framerate << "\n";
        auto p = interpolate(agent_path, i);
        fout << "circle f=1 col=(255,0,0) r=0.3 c=(" << 0.5 + p.first << "," << 0.5 + p.second << ")\n";
        for (auto obstacle : mission.obstacles) {
            auto p = interpolate(obstacle.path, i);
            fout << "circle f=1 col=(0,0,255) r=0.3 c=(" << 0.5 + p.first << "," << 0.5 + p.second << ")\n";
        }
    }
}
