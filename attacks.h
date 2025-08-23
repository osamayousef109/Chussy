
#ifndef ATTACKS_H
#define ATTACKS_H
#include "types.h"
constexpr U64 rankMask(int r) {
 return 0xFFULL << (8*r);
}
constexpr U64 fileMask(int f) {
 return 0x0101010101010101ULL << f;
}
constexpr U64 shiftSouth(U64 b) {return b >> 8;}
constexpr U64 shiftNorth(U64 b) {return b << 8;}

inline U64 pawnsAble2Push(U64 pawns, U64 empty,int color) {
 if (color==WHITE)
  return shiftSouth(empty) & pawns;
 return shiftNorth(empty) & pawns;
}

inline U64 pawnsAble2DblPush(U64 pawns, U64 empty,int color) {
 if (color==WHITE) {
  const U64 rank4 = rankMask(3);
  U64 emptyRank3 = shiftSouth(empty & rank4) & empty;
  return pawnsAble2Push(pawns, emptyRank3, color);
 }
 const U64 rank5 = rankMask(4);
 U64 emptyRank6 = shiftNorth(empty & rank5) & empty;
 return pawnsAble2Push(pawns, emptyRank6, color);
}
void initAttacks();
inline U64 pawnAttacks[2][64];
inline U64 kingAttacks[64];
inline U64 knightAttacks[64];
#endif //ATTACKS_H
