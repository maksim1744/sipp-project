cmake_minimum_required(VERSION 3.2)
project(SIPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-O2")
set(CMAKE_CXX_FLAGS  "-Wfatal-errors")

set(HEADERS
    map.h
    mission.h
    node.h
    node_comparator.h
    obstacle.h
    options.h
    path_node.h
    search.h
    search_result.h
    tinyxml2.h
    viewer_logger.h
    xmllogger.h
)

set(SOURCES
    map.cpp
    mission.cpp
    node_comparator.cpp
    search.cpp
    tinyxml2.cpp
    xmllogger.cpp
    viewer_logger.cpp
)

add_executable(SIPP ${HEADERS} ${SOURCES} main.cpp)
