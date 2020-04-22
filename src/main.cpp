#include <iostream>
#include <algorithm>

#include "mission.h"
#include "viewer_logger.h"
#include "xmllogger.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Need exactly one file" << std::endl;
        return 1;
    }

    Mission mission;
    XmlLogger xmllogger;

    if (!xmllogger.read_from_xml(argv[1], &mission)) {
        std::cerr << "Error while reading from xml" << std::endl;
        return 1;
    }

    if (!mission.run_search()) {
        std::cerr << "Error while searching a path" << std::endl;
        return 1;
    }

    std::string filename(argv[1]);
    if (std::count(filename.begin(), filename.end(), '.') == 0) {
        filename += "_log";
    } else {
        int ind = filename.rfind('.');
        filename.insert(ind, std::string("_log"));
    }

    if (!xmllogger.write_to_xml(filename.c_str(), mission)) {
        std::cerr << "Error while writing to xml" << std::endl;
        return 1;
    }
    ViewerLogger().write_to_file((filename.substr(0, filename.size() - 3) + "txt").c_str(), mission);

    std::cerr << "Success" << std::endl;

    return 0;
}
