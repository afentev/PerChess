//
// Created by afentev on 6/9/21.
//

#ifndef PERCHESS_MEASURES_H
#define PERCHESS_MEASURES_H

#include <cstdint>
#include "Position.h"

uint64_t nortFill(uint64_t sources);

uint64_t soutFill(uint64_t sources);

uint64_t fileFill(uint64_t sources);

uint64_t closedFiles(uint64_t wpanws, uint64_t bpawns);

uint64_t openFiles(uint64_t wpanws, uint64_t bpawns);

uint64_t halfOpenOrOpenFile(uint64_t gen);

uint64_t wHalfOpenFile(uint64_t wpawns, uint64_t bpanws);

uint64_t bHalfOpenFile(uint64_t wpawns, uint64_t bpanws);


#endif //PERCHESS_MEASURES_H
