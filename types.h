//
// Created by MSi on 8/23/2025.
//

#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
using U64 = uint64_t;
using Move = uint32_t;
constexpr uint8_t FLAG_CAPTURE   = 1 << 0;
constexpr uint8_t FLAG_EN_PASSANT= 1 << 1;
constexpr uint8_t FLAG_CASTLE    = 1 << 2;
constexpr uint8_t FLAG_PROMOTION = 1 << 3;
enum Piece {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING,EMPTY};
enum Color {WHITE,BLACK,NO_COLOR};
struct History {
    Move move;
    int captured;
    uint8_t castlingRights;
    int prevEnpassant;
    int captureSquare;
    int halfMoveClock;
};
inline Move make_move(int from,int to,int promo=0,int flags=0) {
    return (from) | (to << 6) | (promo << 12) | (flags << 16);
}
inline int fromSquare(Move m)      { return  (int)m        & 0x3F; }
inline int toSquare(Move m)        { return (int)(m >> 6)  & 0x3F; }
inline int promotionPiece(Move m)  { return (int)(m >> 12) & 0xF;  }
inline int moveFlags(Move m)       { return (int)(m >> 16) & 0x3F; }
#endif //TYPES_H
