//
// Created by MSi on 8/23/2025.
//

#ifndef TT_H
#define TT_H
#include "types.h"


struct PerftEntry {
    U64 key;
    int depth;
    long long nodes;
};
constexpr int TT_SIZE=(1<<20);
inline PerftEntry perftTable[TT_SIZE]; // simple big array

inline bool probePerftTT(U64 key, int depth, long long& nodes) {
    PerftEntry& e = perftTable[key & (TT_SIZE - 1)];
    if (e.key == key && e.depth == depth) {
        nodes = e.nodes;
        return true;
    }
    return false;
}

inline void storePerftTT(U64 key, int depth, long long nodes) {
    PerftEntry& e = perftTable[key & (TT_SIZE - 1)];
    e.key = key;
    e.depth = depth;
    e.nodes = nodes;
}


#endif //TT_H
