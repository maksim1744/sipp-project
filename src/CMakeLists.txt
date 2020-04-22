cmake_minimum_required(VERSION 3.2)
project(SIPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-O2")
set(CMAKE_CXX_FLAGS  "-Wfatal-errors")

set(HEADERS
    map.h
    mission.h
    obstacle.h
    options.h
    path_node.h
    search.h
    search_result.h
    tinyxml2.h
    xmllogger.h
)

set(SOURCES
    map.cpp
    mission.cpp
    search.cpp
    tinyxml2.cpp
    xmllogger.cpp
)

add_executable(SIPP ${HEADERS} ${SOURCES} main.cpp)
