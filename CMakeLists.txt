cmake_minimum_required(VERSION 3.17)
project(proj)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_subdirectory("${PROJECT_SOURCE_DIR}/thc-chess-library-master" EXCLUDE_FROM_ALL)
#add_subdirectory("${PROJECT_SOURCE_DIR}/fathom1")

include_directories(${THC_CHESS_INCLUDE})
#include_directories(${fathom1})

#add_library(clib fathom1/src/tbprobe.c)

add_executable(proj main.cpp Stuff/stuff.cpp Engine/IterativeTablesEngine.cpp UI/interface.cpp fathom1/src/tbprobe.c)

target_link_libraries(proj thc_chess_static sqlite3 Qt5::Core Qt5::Widgets) # or thc_chess for dynamic linking