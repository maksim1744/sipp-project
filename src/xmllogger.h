#ifndef XMLLOGGER_H
#define	XMLLOGGER_H

#include "map.h"
#include "mission.h"
#include "obstacle.h"
#include "path_node.h"
#include "search_result.h"
#include "tinyxml2.h"

#include <iostream>
#include <vector>

class XmlLogger {
public:
    XmlLogger() {}

    bool read_from_xml(const char *filename, Mission *mission);
    bool write_to_xml(const char *filename, const Mission &mission);

private:

    bool read_map(tinyxml2::XMLElement *element, Map *map);
    bool read_options(tinyxml2::XMLElement *element, Options *options);
    bool read_obstacles(tinyxml2::XMLElement *element, std::vector<Obstacle> *obstacles);
    bool check_obstacles_coordinates(const std::vector<Obstacle> &obstacles, const Map &map);

    bool write_result(tinyxml2::XMLElement *element, const SearchResult &search_result);
    bool write_summary(tinyxml2::XMLElement *element, const SearchResult &search_result);
    bool write_path(tinyxml2::XMLElement *element, const std::vector<PathNode> &path);

    tinyxml2::XMLDocument doc;
};

#endif  // XMLLOGGER_H
