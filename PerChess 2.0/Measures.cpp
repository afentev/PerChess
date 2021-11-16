//
// Created by afentev on 6/9/21.
//

#include "Measures.h"

uint64_t nortFill(uint64_t sources) {
    uint64_t gen = 0;
    gen |= (sources <<  8);
    gen |= (sources << 16);
    gen |= (sources << 32);
    return gen;
}

uint64_t soutFill(uint64_t sources) {
    uint64_t gen = 0;
    gen |= (sources >>  8);
    gen |= (sources >> 16);
    gen |= (sources >> 32);
    return gen;
}

uint64_t fileFill(uint64_t sources) {
    return nortFill(sources) | soutFill(sources);
}

uint64_t closedFiles(uint64_t wpanws, uint64_t bpawns) {
    return fileFill(wpanws) & fileFill(bpawns);
}

uint64_t openFiles(uint64_t wpanws, uint64_t bpawns) {
    return ~fileFill(wpanws | bpawns);
}

uint64_t halfOpenOrOpenFile(uint64_t gen) {
    return ~fileFill(gen);
}

uint64_t wHalfOpenFile(uint64_t wpawns, uint64_t bpanws) {
    return halfOpenOrOpenFile(wpawns) ^ openFiles(wpawns, bpanws);
}

uint64_t bHalfOpenFile(uint64_t wpawns, uint64_t bpawns) {
    return halfOpenOrOpenFile(bpawns) ^ openFiles(wpawns, bpawns);
}
