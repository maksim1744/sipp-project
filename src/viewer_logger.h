#ifndef VIEWER_LOGGER_H
#define VIEWER_LOGGER_H

#include "mission.h"

#include <fstream>
#include <iomanip>
#include <iostream>

// this class generates text file, that you can read or pass to my another project https://github.com/maksim1744/Viewer to see nice animation

class ViewerLogger {
public:
    void write_to_file(const char *filename, const Mission &mission);

    const int framerate = 60;

private:
    void write_map(std::ofstream &fout, const Map &map);
    void write_objects(std::ofstream &fout, const Mission &mission);
};

#endif  // VIEWER_LOGGER_H
