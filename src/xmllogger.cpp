#include "xmllogger.h"

using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

bool XmlLogger::read_from_xml(const char *filename, Mission *mission) {
    if (doc.LoadFile(filename) != tinyxml2::XMLError::XML_SUCCESS) {
        std::cerr << "Error opening input XML file" << std::endl;
        return false;
    }

    auto root = doc.FirstChildElement("root");
    if (!root) {
        std::cerr << "Missing tag <root>" << std::endl;
        return false;
    }

    if (!read_map(root->FirstChildElement("map"), &mission->map))
        return false;
    if (!read_obstacles(root->FirstChildElement("obstacles"), &mission->obstacles))
        return false;
    if (!check_obstacles_coordinates(mission->obstacles, mission->map))
        return false;

    return true;
}

bool XmlLogger::read_map(tinyxml2::XMLElement *element, Map *map) {
    if (!element) {
        std::cerr << "Missing tag <map>" << std::endl;
        return false;
    }
    for (auto tag : {"width", "height", "startx", "starty", "finishx", "finishy", "grid"}) {
        if (!element->FirstChildElement(tag)) {
            std::cerr << "Missing tag <" << tag << ">" << std::endl;
            return false;
        }
    }
    map->width = std::strtol(element->FirstChildElement("width")->GetText(), NULL, 10);
    map->height = std::strtol(element->FirstChildElement("height")->GetText(), NULL, 10);
    map->startx = std::strtol(element->FirstChildElement("startx")->GetText(), NULL, 10);
    map->starty = std::strtol(element->FirstChildElement("starty")->GetText(), NULL, 10);
    map->finishx = std::strtol(element->FirstChildElement("finishx")->GetText(), NULL, 10);
    map->finishy = std::strtol(element->FirstChildElement("finishy")->GetText(), NULL, 10);

    if (map->width <= 0 || map->height <= 0) {
        std::cerr << "Width and height must be positive" << std::endl;
        return false;
    }
    if (map->startx < 0  || map->startx >= map->width  || map->starty < 0  || map->starty >= map->height ||
        map->finishx < 0 || map->finishx >= map->width || map->finishy < 0 || map->finishy >= map->height) {
        std::cerr << "Start and finish must be inside a map" << std::endl;
        return false;
    }

    map->walls.assign(map->height, std::vector<bool>());
    auto row = element->FirstChildElement("grid")->FirstChildElement("row");
    for (int i = 0; i < map->height; ++i) {
        if (!row) {
            std::cerr << "Missing tag <row>" << std::endl;
            return false;
        }
        for (char c : std::string(row->GetText())) {
            if (c == '0') map->walls[i].push_back(false);
            if (c == '1') map->walls[i].push_back(true);
        }
        if (map->walls[i].size() != map->width) {
            std::cerr << "Wrong width of row " << i << std::endl;
            return false;
        }
        row = row->NextSiblingElement();
    }

    return true;
}

bool XmlLogger::read_obstacles(tinyxml2::XMLElement *element, std::vector<Obstacle> *obstacles) {
    if (!element) {
        // no dynamic obstacles -- ok
        return true;
    }
    obstacles->clear();
    for (auto obstacle = element->FirstChildElement("obstacle"); obstacle; obstacle = obstacle->NextSiblingElement()) {
        obstacles->emplace_back();
        for (auto node = obstacle->FirstChildElement("node"); node; node = node->NextSiblingElement()) {
            int x = node->IntAttribute("x", -1);
            int y = node->IntAttribute("y", -1);
            obstacles->back().path.emplace_back(x, y);
        }
        if (obstacles->back().path.empty()) {
            std::cerr << "No nodes found in obstacle" << std::endl;
            return false;
        }
        for (int i = 0; i + 1 < obstacles->back().path.size(); ++i) {
            int dist = abs(obstacles->back().path[i].first  - obstacles->back().path[i + 1].first) + 
                       abs(obstacles->back().path[i].second - obstacles->back().path[i + 1].second);
            if (dist > 1) {
                std::cerr << "Neighboring nodes are too far" << std::endl;
            }
        }
    }
    return true;
}

bool XmlLogger::check_obstacles_coordinates(const std::vector<Obstacle> &obstacles, const Map &map) {
    for (auto &obstacle : obstacles) {
        for (auto [x, y] : obstacle.path) {
            if (x < 0 || y < 0 || x >= map.width || y >= map.height) {
                std::cerr << "Obstacle coordinates must be inside a map" << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool XmlLogger::write_to_xml(const char *filename, const Mission &mission) {
    auto root = doc.FirstChildElement("root");
    root->InsertEndChild(doc.NewElement("log"));
    root = root->LastChildElement("log");

    if (!write_result(root, mission.search_result))
        return false;

    if (mission.search_result.path_found) {
        if (!write_summary(root, mission.search_result))
            return false;

        if (!write_path(root, mission.search_result.path))
            return false;
    }

    doc.SaveFile(filename);
    return true;
}

bool XmlLogger::write_result(XMLElement *element, const SearchResult &search_result) {
    element->InsertEndChild(doc.NewElement("result"));
    element = element->LastChildElement("result");
    if (search_result.path_found)
        element->SetText("Path found");
    else
        element->SetText("Path not found");

    return true;
}

bool XmlLogger::write_summary(XMLElement *element, const SearchResult &search_result) {
    element->InsertEndChild(doc.NewElement("summary"));
    element = element->LastChildElement("summary");
    return true;
}

bool XmlLogger::write_path(XMLElement *element, const std::vector<PathNode> &path) {
    element->InsertEndChild(doc.NewElement("path"));
    element = element->LastChildElement("path");
    for (auto &node : path) {
        element->InsertEndChild(doc.NewElement("node"));
        auto node_element = element->LastChildElement("node");
        node_element->SetAttribute("x", node.x);
        node_element->SetAttribute("y", node.y);
        node_element->SetAttribute("time", node.time);
    }
    return true;
}
