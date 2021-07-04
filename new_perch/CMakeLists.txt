cmake_minimum_required(VERSION 3.19)
project(PerChess)

set(CMAKE_CXX_STANDARD 20)

add_executable(PerChess main.cpp Position.cpp Position.h PawnsBitboardsFiles/PawnsAttacks.h
        SlidingBitboardsFiles/slider_attacks.cpp PawnsBitboardsFiles/PawnAttacks.cpp
        KnightsBitboardsFiles/KnightsMoves.cpp KnightsBitboardsFiles/KnightsMoves.h
        KingBitboardsFiles/KingMoves.cpp KingBitboardsFiles/KingMoves.h Engine.cpp Engine.h Tables.cpp Tables.h
        Measures.cpp Measures.h MobilityFiles/Mobility.cpp MobilityFiles/Mobility.h MovesJumpTable.cpp MovesJumpTable.h)

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
