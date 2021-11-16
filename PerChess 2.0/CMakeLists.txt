cmake_minimum_required(VERSION 3.19)
project(PerChess)

set(CMAKE_CXX_STANDARD 20)

add_executable(PerChess main.cpp Position.cpp
        SlidingBitboardsFiles/slider_attacks.cpp PawnsBitboardsFiles/PawnAttacks.cpp
        KnightsBitboardsFiles/KnightsMoves.cpp
        KingBitboardsFiles/KingMoves.cpp Engine.cpp Tables.cpp
        Measures.cpp MobilityFiles/Mobility.cpp MovesJumpTable.cpp)

#set(GCC_COVERAGE_COMPILE_FLAGS "-fopenmp")
#set(GCC_COVERAGE_COMPILE_FLAGS "-lpthread")
#
#set(OMP_CANCELLATION, "true")
#
#find_package(OpenMP)
#if(OpenMP_CXX_FOUND)
#    target_link_libraries(PerChess PUBLIC OpenMP::OpenMP_CXX)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -mpopcnt -funroll-loops -march=icelake-client")

#pragma GCC target("popcnt")

#SET(OMP_CANCELLATION, "true")
#setenv("OMP_CANCELLATION", "true", 1)
