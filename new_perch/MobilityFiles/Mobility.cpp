//
// Created by afentev on 6/9/21.
//

#include "Mobility.h"

std::vector<std::vector<std::vector<int>>> mobilityTable {
        { {-38,-33}, {-25,-23}, {-12,-13}, { 0, -3}, {12,  7},
                {25, 17}, { 31, 22}, { 38, 27}, { 38, 27} },  // Knights

        {},

        { {-20,-36}, {-14,-19}, { -8, -3}, {-2, 13}, { 4, 29},
                {10, 46}, { 14, 62}, { 19, 79}, { 23, 95}, {26,106},
                {27,111}, {28,114}, { 29,116}, { 30,117}, { 31,118},
                {32,118} },  // Rooks

        {},

        { {-25,-30}, {-11,-16}, {  3, -2}, {17, 12}, {31, 26},
                {45, 40}, { 57, 52}, { 65, 60}, { 71, 65}, {74, 69},
                {76, 71}, {78, 73}, { 79, 74}, { 80, 75}, { 81, 76},
                {81, 76} },  // Bishops

        { {-10,-18}, { -8,-13}, { -6, -7}, {-3, -2}, {-1,  3},
                { 1,  8}, {  3, 13}, {  5, 19}, {  8, 23}, {10, 27},
                {12, 32}, {15, 34}, { 16, 35}, { 17, 35}, { 18, 35},
                {20, 35}, {20, 35}, {20, 35}, { 20, 35}, { 20, 35},
                { 20, 35}, {20, 35}, {20, 35}, {20, 35}, { 20, 35},
                { 20, 35}, { 20, 35}, {20, 35}, {20, 35}, {20, 35},
                { 20, 35}, { 20, 35} }  // Queens
};