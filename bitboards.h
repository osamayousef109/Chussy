
#ifndef BITBOARDS_H
#define BITBOARDS_H
#include <sstream>
#include <string>

#include "attacks.h"
#include "types.h"
struct SMagic {
    U64* ptr;  // pointer to attack_table for each particular square
    U64 mask;  // to mask relevant squares of both lines (no outer squares)
    U64 magic; // magic 64-bit factor
    int shift; // shift right
};
inline SMagic mBishopTbl[64];
inline SMagic mRookTbl[64];
constexpr U64 bishopAttacks(int sq,U64 occ) {
    U64* aptr = mBishopTbl[sq].ptr;
    occ      &= mBishopTbl[sq].mask;
    occ      *= mBishopTbl[sq].magic;
    occ     >>= mBishopTbl[sq].shift;
    return aptr[occ];
}

constexpr U64 rookAttacks(int sq,U64 occ) {
    U64* aptr = mRookTbl[sq].ptr;
    occ      &= mRookTbl[sq].mask;
    occ      *= mRookTbl[sq].magic;
    occ     >>= mRookTbl[sq].shift;
    return aptr[occ];
}

constexpr bool inBoard(int r,int c){return r>=0&&r<8&&c>=0&&c<8;}
inline int bishopDirections[4][2]={{1,1},{-1,1},{1,-1},{-1,-1}};
inline int rookDirections[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
constexpr U64 bishopAttackRay(int sq,U64 occ) {
    int r=sq/8;
    int c=sq%8;
    U64 out=0;
    for (auto& m:bishopDirections) {
        for (int step=1;step<=7;step++) {
            int nr=r+step*m[0];
            int nc=c+step*m[1];
            if (!inBoard(nr,nc)) break;
            int to=nr*8+nc;
            out|=(1ULL<<to);
            if ((1ULL<<to)&occ) break;
        }
    }
    return out;
}
constexpr U64 rookAttackRay(int sq, U64 occ) {
    int r=sq/8;
    int c=sq%8;
    U64 out=0;
    for (auto& m:rookDirections) {
        for (int step=1;step<=7;step++) {
            int nr=r+step*m[0];
            int nc=c+step*m[1];
            if (!inBoard(nr,nc)) break;
            int to=nr*8+nc;
            out|=(1ULL<<to);
            if ((1ULL<<to)&occ) break;
        }
    }
    return out;
}
void initMagic();

inline Move moves[512];
inline History history[512];
inline int moveIdx=0;
inline int historyIdx=0;


struct Board {
    U64 piece[2][6];
    U64 occupied[2];
    U64 allOccupied;
    U64 empty;
    int kingPos[2]{4,60};
    int mailbox[64] = {
        ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK,
        PAWN,  PAWN,   PAWN,   PAWN,  PAWN,  PAWN,   PAWN,   PAWN,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        PAWN,  PAWN,   PAWN,   PAWN,  PAWN,  PAWN,   PAWN,   PAWN,
        ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK
    };
    int enpassant=-1;
    Color currentColor=WHITE;
    uint8_t castlingRights=15;
    int halfMoveClock=0;
    void setFen(const std::string& fen) {
        for (int c = 0; c < 2; ++c) {
            for (int p = 0; p < 6; ++p) {
                piece[c][p] = 0ULL;
            }
            occupied[c] = 0ULL;
            kingPos[c] = -1;
        }
        for (int i = 0; i < 64; ++i) {
            mailbox[i] = EMPTY;
        }
        std::stringstream ss(fen);
        std::string placement, active_color, castling, en_passant_str;
        ss >> placement >> active_color >> castling >> en_passant_str;
        int rank = 7, file = 0;
        for (char ch : placement) {
            if (isalpha(ch)) {
                int sq = rank * 8 + file;
                Color p_color = isupper(ch) ? WHITE : BLACK;
                Piece p_type;
                switch (tolower(ch)) {
                    case 'p': p_type = PAWN; break;
                    case 'n': p_type = KNIGHT; break;
                    case 'b': p_type = BISHOP; break;
                    case 'r': p_type = ROOK; break;
                    case 'q': p_type = QUEEN; break;
                    case 'k': p_type = KING; kingPos[p_color] = sq; break;
                    default: continue;
                }
                U64 bb = 1ULL << sq;
                piece[p_color][p_type] |= bb;
                occupied[p_color] |= bb;
                mailbox[sq] = p_type;
                file++;
            } else if (isdigit(ch)) {
                file += (ch - '0');
            } else if (ch == '/') {
                rank--;
                file = 0;
            }
        }
        allOccupied = occupied[WHITE] | occupied[BLACK];
        empty = ~allOccupied;
        currentColor = (active_color == "w") ? WHITE : BLACK;
        castlingRights = 0;
        for (char ch : castling) {
            switch (ch) {
                case 'K': castlingRights |= (1 << 0); break;
                case 'Q': castlingRights |= (1 << 1); break;
                case 'k': castlingRights |= (1 << 2); break;
                case 'q': castlingRights |= (1 << 3); break;
            }
        }
        if (en_passant_str != "-") {
            int ep_file = en_passant_str[0] - 'a';
            int ep_rank = en_passant_str[1] - '1';
            enpassant = ep_rank * 8 + ep_file;
        } else {
            enpassant = -1;
        }
        ss >> halfMoveClock;
    }
    Board() {
        piece[WHITE][PAWN] = rankMask(1);
        piece[BLACK][PAWN] = rankMask(6);

        piece[WHITE][KNIGHT] = (1ULL << 1) | (1ULL << 6);
        piece[BLACK][KNIGHT] = (1ULL << 57) | (1ULL << 62);

        piece[WHITE][BISHOP] = (1ULL << 2) | (1ULL << 5);
        piece[BLACK][BISHOP] = (1ULL << 58) | (1ULL << 61);

        piece[WHITE][ROOK] = (1ULL << 0) | (1ULL << 7);
        piece[BLACK][ROOK] = (1ULL << 56) | (1ULL << 63);

        piece[WHITE][QUEEN] = (1ULL << 3);
        piece[BLACK][QUEEN] = (1ULL << 59);

        piece[WHITE][KING] = (1ULL << 4);
        piece[BLACK][KING] = (1ULL << 60);

        occupied[WHITE] = piece[WHITE][PAWN] | piece[WHITE][KNIGHT] | piece[WHITE][BISHOP] | piece[WHITE][ROOK] | piece[WHITE][QUEEN] | piece[WHITE][KING];
        occupied[BLACK] = piece[BLACK][PAWN] | piece[BLACK][KNIGHT] | piece[BLACK][BISHOP] | piece[BLACK][ROOK] | piece[BLACK][QUEEN] | piece[BLACK][KING];
        allOccupied = occupied[WHITE] | occupied[BLACK];
        empty=~allOccupied;
    }
};
inline bool isAttacked(Board& board,int color,int sq) {
    int enemy=1-color;
    if (pawnAttacks[color][sq]&board.piece[enemy][PAWN]) return true;
    if (knightAttacks[sq]&board.piece[enemy][KNIGHT]) return true;
    if (kingAttacks[sq]&board.piece[enemy][KING]) return true;
    U64 moves = bishopAttacks(sq,board.allOccupied);
    if (moves&(board.piece[enemy][BISHOP]|board.piece[enemy][QUEEN])) return true;
    moves= rookAttacks(sq,board.allOccupied);
    if (moves&(board.piece[enemy][ROOK]|board.piece[enemy][QUEEN])) return true;
    return false;
}
#endif //BITBOARDS_H
