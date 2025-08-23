#include <bits/stdc++.h>
using namespace std;
using U64 = uint64_t;
using Move = uint32_t;
constexpr uint8_t FLAG_CAPTURE   = 1 << 0;
constexpr uint8_t FLAG_EN_PASSANT= 1 << 1;
constexpr uint8_t FLAG_CASTLE    = 1 << 2;
constexpr uint8_t FLAG_PROMOTION = 1 << 3;
enum Piece {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING,EMPTY};
enum Color {WHITE,BLACK,NO_COLOR};


const U64 RMagic[64] = {
  0x80004000976080ULL,
  0x1040400010002000ULL,
  0x4880200210000980ULL,
  0x5280080010000482ULL,
  0x200040200081020ULL,
  0x2100080100020400ULL,
  0x4280008001000200ULL,
  0x1000a4425820300ULL,
  0x29002100800040ULL,
  0x4503400040201004ULL,
  0x209002001004018ULL,
  0x1131000a10002100ULL,
  0x9000800120500ULL,
  0x10e001804820010ULL,
  0x29000402000100ULL,
  0x2002000d01c40292ULL,
  0x80084000200c40ULL,
  0x10004040002002ULL,
  0x201030020004014ULL,
  0x80012000a420020ULL,
  0x129010008001204ULL,
  0x6109010008040002ULL,
  0x950010100020004ULL,
  0x803a0000c50284ULL,
  0x80004100210080ULL,
  0x200240100140ULL,
  0x20004040100800ULL,
  0x4018090300201000ULL,
  0x4802010a00102004ULL,
  0x2001000900040002ULL,
  0x4a02104001002a8ULL,
  0x2188108200204401ULL,
  0x40400020800080ULL,
  0x880402000401004ULL,
  0x10040800202000ULL,
  0x604410a02001020ULL,
  0x200200206a001410ULL,
  0x86000400810080ULL,
  0x428200040600080bULL,
  0x2001000041000082ULL,
  0x80002000484000ULL,
  0x210002002c24000ULL,
  0x401a200100410014ULL,
  0x5021000a30009ULL,
  0x218000509010010ULL,
  0x4000400410080120ULL,
  0x20801040010ULL,
  0x29040040820011ULL,
  0x4080400024800280ULL,
  0x500200040100440ULL,
  0x2880142001004100ULL,
  0x412020400a001200ULL,
  0x18c028004080080ULL,
  0x884001020080401ULL,
  0x210810420400ULL,
  0x801048745040200ULL,
  0x4401002040120082ULL,
  0x408200210012ULL,
  0x110008200441ULL,
  0x2010002004100901ULL,
  0x801000800040211ULL,
  0x480d000400820801ULL,
  0x820104201280084ULL,
  0x1001040311802142ULL,
};

const U64 BMagic[64] = {
  0x1024b002420160ULL,
  0x1008080140420021ULL,
  0x2012080041080024ULL,
  0xc282601408c0802ULL,
  0x2004042000000002ULL,
  0x12021004022080ULL,
  0x880414820100000ULL,
  0x4501002211044000ULL,
  0x20402222121600ULL,
  0x1081088a28022020ULL,
  0x1004c2810851064ULL,
  0x2040080841004918ULL,
  0x1448020210201017ULL,
  0x4808110108400025ULL,
  0x10504404054004ULL,
  0x800010422092400ULL,
  0x40000870450250ULL,
  0x402040408080518ULL,
  0x1000980a404108ULL,
  0x1020804110080ULL,
  0x8200c02082005ULL,
  0x40802009a0800ULL,
  0x1000201012100ULL,
  0x111080200820180ULL,
  0x904122104101024ULL,
  0x4008200405244084ULL,
  0x44040002182400ULL,
  0x4804080004021002ULL,
  0x6401004024004040ULL,
  0x404010001300a20ULL,
  0x428020200a20100ULL,
  0x300460100420200ULL,
  0x404200c062000ULL,
  0x22101400510141ULL,
  0x104044400180031ULL,
  0x2040040400280211ULL,
  0x8020400401010ULL,
  0x20100110401a0040ULL,
  0x100101005a2080ULL,
  0x1a008300042411ULL,
  0x120a025004504000ULL,
  0x4001084242101000ULL,
  0xa020202010a4200ULL,
  0x4000002018000100ULL,
  0x80104000044ULL,
  0x1004009806004043ULL,
  0x100401080a000112ULL,
  0x1041012101000608ULL,
  0x40400c250100140ULL,
  0x80a10460a100002ULL,
  0x2210030401240002ULL,
  0x6040aa108481b20ULL,
  0x4009004050410002ULL,
  0x8106003420200e0ULL,
  0x1410500a08206000ULL,
  0x92548802004000ULL,
  0x1040041241028ULL,
  0x120042025011ULL,
  0x8060104054400ULL,
  0x20004404020a0a01ULL,
  0x40008010020214ULL,
  0x4000050209802c1ULL,
  0x208244210400ULL,
  0x10140848044010ULL,
};
int RBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
  };

int BBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
  };
const U64 RMask[64] = {
  0x000101010101017eULL, // pop=12
  0x000202020202027cULL, // pop=11
  0x000404040404047aULL, // pop=11
  0x0008080808080876ULL, // pop=11
  0x001010101010106eULL, // pop=11
  0x002020202020205eULL, // pop=11
  0x004040404040403eULL, // pop=11
  0x008080808080807eULL, // pop=12
  0x0001010101017e00ULL, // pop=11
  0x0002020202027c00ULL, // pop=10
  0x0004040404047a00ULL, // pop=10
  0x0008080808087600ULL, // pop=10
  0x0010101010106e00ULL, // pop=10
  0x0020202020205e00ULL, // pop=10
  0x0040404040403e00ULL, // pop=10
  0x0080808080807e00ULL, // pop=11
  0x00010101017e0100ULL, // pop=11
  0x00020202027c0200ULL, // pop=10
  0x00040404047a0400ULL, // pop=10
  0x0008080808760800ULL, // pop=10
  0x00101010106e1000ULL, // pop=10
  0x00202020205e2000ULL, // pop=10
  0x00404040403e4000ULL, // pop=10
  0x00808080807e8000ULL, // pop=11
  0x000101017e010100ULL, // pop=11
  0x000202027c020200ULL, // pop=10
  0x000404047a040400ULL, // pop=10
  0x0008080876080800ULL, // pop=10
  0x001010106e101000ULL, // pop=10
  0x002020205e202000ULL, // pop=10
  0x004040403e404000ULL, // pop=10
  0x008080807e808000ULL, // pop=11
  0x0001017e01010100ULL, // pop=11
  0x0002027c02020200ULL, // pop=10
  0x0004047a04040400ULL, // pop=10
  0x0008087608080800ULL, // pop=10
  0x0010106e10101000ULL, // pop=10
  0x0020205e20202000ULL, // pop=10
  0x0040403e40404000ULL, // pop=10
  0x0080807e80808000ULL, // pop=11
  0x00017e0101010100ULL, // pop=11
  0x00027c0202020200ULL, // pop=10
  0x00047a0404040400ULL, // pop=10
  0x0008760808080800ULL, // pop=10
  0x00106e1010101000ULL, // pop=10
  0x00205e2020202000ULL, // pop=10
  0x00403e4040404000ULL, // pop=10
  0x00807e8080808000ULL, // pop=11
  0x007e010101010100ULL, // pop=11
  0x007c020202020200ULL, // pop=10
  0x007a040404040400ULL, // pop=10
  0x0076080808080800ULL, // pop=10
  0x006e101010101000ULL, // pop=10
  0x005e202020202000ULL, // pop=10
  0x003e404040404000ULL, // pop=10
  0x007e808080808000ULL, // pop=11
  0x7e01010101010100ULL, // pop=12
  0x7c02020202020200ULL, // pop=11
  0x7a04040404040400ULL, // pop=11
  0x7608080808080800ULL, // pop=11
  0x6e10101010101000ULL, // pop=11
  0x5e20202020202000ULL, // pop=11
  0x3e40404040404000ULL, // pop=11
  0x7e80808080808000ULL, // pop=12
};

const U64 BMask[64] = {
  0x0040201008040200ULL, // pop=6
  0x0000402010080400ULL, // pop=5
  0x0000004020100a00ULL, // pop=5
  0x0000000040221400ULL, // pop=5
  0x0000000002442800ULL, // pop=5
  0x0000000204085000ULL, // pop=5
  0x0000020408102000ULL, // pop=5
  0x0002040810204000ULL, // pop=6
  0x0020100804020000ULL, // pop=5
  0x0040201008040000ULL, // pop=5
  0x00004020100a0000ULL, // pop=5
  0x0000004022140000ULL, // pop=5
  0x0000000244280000ULL, // pop=5
  0x0000020408500000ULL, // pop=5
  0x0002040810200000ULL, // pop=5
  0x0004081020400000ULL, // pop=5
  0x0010080402000200ULL, // pop=5
  0x0020100804000400ULL, // pop=5
  0x004020100a000a00ULL, // pop=7
  0x0000402214001400ULL, // pop=7
  0x0000024428002800ULL, // pop=7
  0x0002040850005000ULL, // pop=7
  0x0004081020002000ULL, // pop=5
  0x0008102040004000ULL, // pop=5
  0x0008040200020400ULL, // pop=5
  0x0010080400040800ULL, // pop=5
  0x0020100a000a1000ULL, // pop=7
  0x0040221400142200ULL, // pop=9
  0x0002442800284400ULL, // pop=9
  0x0004085000500800ULL, // pop=7
  0x0008102000201000ULL, // pop=5
  0x0010204000402000ULL, // pop=5
  0x0004020002040800ULL, // pop=5
  0x0008040004081000ULL, // pop=5
  0x00100a000a102000ULL, // pop=7
  0x0022140014224000ULL, // pop=9
  0x0044280028440200ULL, // pop=9
  0x0008500050080400ULL, // pop=7
  0x0010200020100800ULL, // pop=5
  0x0020400040201000ULL, // pop=5
  0x0002000204081000ULL, // pop=5
  0x0004000408102000ULL, // pop=5
  0x000a000a10204000ULL, // pop=7
  0x0014001422400000ULL, // pop=7
  0x0028002844020000ULL, // pop=7
  0x0050005008040200ULL, // pop=7
  0x0020002010080400ULL, // pop=5
  0x0040004020100800ULL, // pop=5
  0x0000020408102000ULL, // pop=5
  0x0000040810204000ULL, // pop=5
  0x00000a1020400000ULL, // pop=5
  0x0000142240000000ULL, // pop=5
  0x0000284402000000ULL, // pop=5
  0x0000500804020000ULL, // pop=5
  0x0000201008040200ULL, // pop=5
  0x0000402010080400ULL, // pop=5
  0x0002040810204000ULL, // pop=6
  0x0004081020400000ULL, // pop=5
  0x000a102040000000ULL, // pop=5
  0x0014224000000000ULL, // pop=5
  0x0028440200000000ULL, // pop=5
  0x0050080402000000ULL, // pop=5
  0x0020100804020000ULL, // pop=5
  0x0040201008040200ULL, // pop=6
};
U64 attack_table[107648]; // ~840 KiB all rook and bishop attacks, less with constructive collisions optimization

struct SMagic {
    U64* ptr;  // pointer to attack_table for each particular square
    U64 mask;  // to mask relevant squares of both lines (no outer squares)
    U64 magic; // magic 64-bit factor
    int shift; // shift right
};

SMagic mBishopTbl[64];
SMagic mRookTbl[64];

U64 bishopAttacks(int sq,U64 occ) {
    U64* aptr = mBishopTbl[sq].ptr;
    occ      &= mBishopTbl[sq].mask;
    occ      *= mBishopTbl[sq].magic;
    occ     >>= mBishopTbl[sq].shift;
    return aptr[occ];
}

U64 rookAttacks(int sq,U64 occ) {
    U64* aptr = mRookTbl[sq].ptr;
    occ      &= mRookTbl[sq].mask;
    occ      *= mRookTbl[sq].magic;
    occ     >>= mRookTbl[sq].shift;
    return aptr[occ];
}

inline bool inBoard(int r,int c){return r>=0&&r<8&&c>=0&&c<8;}
int bishopDirections[4][2]={{1,1},{-1,1},{1,-1},{-1,-1}};
int rookDirections[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
U64 bishopAttackRay(int sq,U64 occ) {
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
U64 rookAttackRay(int sq, U64 occ) {
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
void initMagic() {
    U64* ptr = attack_table;
    for (int sq = 0; sq < 64; ++sq) {
        int bits = RBits[sq];
        const int tableSize = 1 << bits;
        U64 mask = RMask[sq];
        U64 magic = RMagic[sq];
        mRookTbl[sq].magic = magic;
        mRookTbl[sq].mask  = mask;
        mRookTbl[sq].ptr   = ptr;
        mRookTbl[sq].shift = 64 - bits;
        for (U64 s = mask; ; s = (s - 1) & mask) {
            U64 occ = s;
            int idx = (int)((occ * magic) >> (64 - bits));
            U64 att = rookAttackRay(sq, occ);
            ptr[idx] = att;
            if (s == 0) break;
        }
        ptr += tableSize;
    }
    for (int sq = 0; sq < 64; ++sq) {
        int bits = BBits[sq];
        const int tableSize = 1 << bits;
        U64 mask = BMask[sq];
        U64 magic = BMagic[sq];
        mBishopTbl[sq].magic = magic;
        mBishopTbl[sq].mask  = mask;
        mBishopTbl[sq].ptr   = ptr;
        mBishopTbl[sq].shift = 64 - bits;
        for (U64 s = mask; ; s = (s - 1) & mask) {
            U64 occ = s;
            int idx = (int)((occ * magic) >> (64 - bits));
            U64 att = bishopAttackRay(sq, occ);
            ptr[idx] = att;
            if (s == 0) break;
        }
        ptr += tableSize;
    }
}


constexpr U64 rankMask(int r) {
    return 0xFFULL << (8*r);
}
constexpr U64 fileMask(int f) {
    return 0x0101010101010101ULL << f;
}
U64 shiftSouth(U64 b) {return b >> 8;}
U64 shiftNorth(U64 b) {return b << 8;}

U64 pawnsAble2Push(U64 pawns, U64 empty,int color) {
    if (color==WHITE)
        return shiftSouth(empty) & pawns;
    return shiftNorth(empty) & pawns;
}

U64 pawnsAble2DblPush(U64 pawns, U64 empty,int color) {
    if (color==WHITE) {
        const U64 rank4 = rankMask(3);
        U64 emptyRank3 = shiftSouth(empty & rank4) & empty;
        return pawnsAble2Push(pawns, emptyRank3, color);
    }
    const U64 rank5 = rankMask(4);
    U64 emptyRank6 = shiftNorth(empty & rank5) & empty;
    return pawnsAble2Push(pawns, emptyRank6, color);
}
struct History {
    Move move;
    int captured;
    uint8_t castlingRights;
    int prevEnpassant;
    int captureSquare;
    int halfMoveClock;
};
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
        // Clear all board state
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

        // 1. Piece Placement
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

        // 2. Active Color
        currentColor = (active_color == "w") ? WHITE : BLACK;

        // 3. Castling Rights
        castlingRights = 0;
        for (char ch : castling) {
            switch (ch) {
                case 'K': castlingRights |= (1 << 0); break;
                case 'Q': castlingRights |= (1 << 1); break;
                case 'k': castlingRights |= (1 << 2); break;
                case 'q': castlingRights |= (1 << 3); break;
            }
        }

        // 4. En Passant Square
        if (en_passant_str != "-") {
            int ep_file = en_passant_str[0] - 'a';
            int ep_rank = en_passant_str[1] - '1';
            enpassant = ep_rank * 8 + ep_file;
        } else {
            enpassant = -1;
        }

        // 5. Halfmove Clock & Fullmove Number
        ss >> halfMoveClock;
        // The fullmove number is parsed but not stored as the struct doesn't have a member for it.
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
Move moves[512];
History history[512];
int moveIdx=0;
int historyIdx=0;
inline Move make_move(int from,int to,int promo=0,int flags=0) {
    return (from) | (to << 6) | (promo << 12) | (flags << 16);
}
inline int fromSquare(Move m)      { return  m        & 0x3F; }
inline int toSquare(Move m)        { return (m >> 6)  & 0x3F; }
inline int promotionPiece(Move m)  { return (m >> 12) & 0xF;  }
inline int moveFlags(Move m)       { return (m >> 16) & 0x3F; }

U64 pawnAttacks[2][64];
U64 kingAttacks[64];
U64 knightAttacks[64];
void initAttacks() {
    for (int sq=0;sq<64;sq++) {
        U64 nBB=0ULL;
        U64 wpBB=0ULL;
        U64 bpBB=0ULL;
        U64 kBB=0ULL;
        int rank=sq/8,file=sq%8;
        int knight[8][2]={
            {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
            {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
        };
        int king [8][2]={
            {1,0},{-1,0},{0,1},{0,-1},
            {1,1},{1,-1},{-1,1},{-1,-1}
        };
        for (auto& m:knight) {
            int r = rank + m[0], f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8)
                nBB |= 1ULL << (r * 8 + f);
        }
        for (auto& m:king) {
            int r = rank + m[0], f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8)
                kBB |= 1ULL << (r * 8 + f);
        }
        int wPawn[2][2]={
            {1,1},{1,-1}
        };
        int bPawn[2][2]={
            {-1,1},{-1,-1}
        };
        for (auto& m:wPawn) {
            int r = rank + m[0], f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8)
                wpBB |= 1ULL << (r * 8 + f);
        }
        for (auto& m:bPawn) {
            int r = rank + m[0], f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8)
                bpBB |= 1ULL << (r * 8 + f);
        }
        knightAttacks[sq] = nBB;
        pawnAttacks[WHITE][sq]=wpBB;
        pawnAttacks[BLACK][sq]=bpBB;
        kingAttacks[sq]=kBB;
    }
}

void printBitboard(U64 bb) {
    for (int rank=7;rank>=0;rank--) {
        cout << rank+1 << "  ";
        for (int file=0;file<8;file++) {
            int sq=rank*8+file; //
            cout << ((bb>>sq)&1ULL? "1 " : ". ");
        }
        cout << "\n";
    }
    cout << "\n   a b c d e f g h\n\n";
}
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
void makeMove(Board& board,Move move) {
    History h{};
    int from=fromSquare(move);
    int to=toSquare(move);
    int promo=promotionPiece(move);
    int flag=moveFlags(move);
    int piece=board.mailbox[from];
    int color=board.currentColor;
    int enemy=1-color;
    h.captured=board.mailbox[to];
    h.prevEnpassant=board.enpassant;
    h.captureSquare=to;
    if (!(flag&FLAG_CAPTURE)) h.captureSquare=-1;
    h.move=move;
    h.castlingRights=board.castlingRights;
    h.halfMoveClock=board.halfMoveClock;
    U64 fromBB=(1ULL<<from);
    U64 toBB=(1ULL<<to);
    if (flag&(FLAG_CAPTURE)&&!(flag&FLAG_EN_PASSANT)) {
        board.piece[enemy][h.captured]&=~(toBB);
        board.occupied[enemy]&=~(toBB);
    }
    board.mailbox[to]=piece;
    board.mailbox[from]=EMPTY;
    board.piece[color][piece]&=~fromBB;
    board.allOccupied&=~fromBB;
    board.occupied[color]&=~fromBB;
    board.piece[color][piece]|=toBB;
    board.allOccupied|=toBB;
    board.occupied[color]|=toBB;
    if (flag&(FLAG_EN_PASSANT)) {
        int back=(color==WHITE? -8:8);
        back+=to;
        h.captured=board.mailbox[back];
        board.mailbox[back]=EMPTY;
        h.captureSquare=back;
        U64 backBB=(1ULL<<back);
        board.allOccupied&=~backBB;
        board.piece[enemy][PAWN]&=~backBB;
        board.occupied[enemy]&=~backBB;
    }
    if (flag&(FLAG_CASTLE)) {
        if (to>from) {
            int end=7;
            if (color) end=63;
            board.mailbox[end]=EMPTY;
            board.mailbox[end-2]=ROOK;
            U64 endBB=(1ULL<<end);
            U64 finBB=(1ULL<<(end-2));
            board.piece[color][ROOK]&=~endBB;
            board.piece[color][ROOK]|=finBB;
            board.allOccupied&=~endBB;
            board.allOccupied|=finBB;
            board.occupied[color]&=~endBB;
            board.occupied[color]|=finBB;
        }else {
            int end=0;
            if (color) end=56;
            board.mailbox[end]=EMPTY;
            board.mailbox[end+3]=ROOK;
            U64 endBB=(1ULL<<end);
            U64 finBB=(1ULL<<(end+3));
            board.piece[color][ROOK]&=~endBB;
            board.piece[color][ROOK]|=finBB;
            board.allOccupied&=~endBB;
            board.allOccupied|=finBB;
            board.occupied[color]&=~endBB;
            board.occupied[color]|=finBB;
        }
    }
    if (promo) {
        board.mailbox[to]=promo;
        board.piece[color][piece]&=~toBB;
        board.piece[color][promo]|=toBB;
    }
    if (piece!=PAWN&&!(flag&(FLAG_CAPTURE)))
        board.halfMoveClock++;
    else
        board.halfMoveClock=0;
    if (piece==KING) {
        if (color==WHITE)
            board.castlingRights&=12;
        else
            board.castlingRights&=3;
        board.kingPos[color]=to;
    }

    if (from==0||to==0)
        board.castlingRights&=13;
    if (from==7||to==7)
        board.castlingRights&=14;
    if (from==56||to==56)
        board.castlingRights&=7;
    if (from==63||to==63)
        board.castlingRights&=11;


    if (piece==PAWN&&abs(to/8-from/8)==2) {
        int back=(board.currentColor==WHITE? -8:8);
        board.enpassant=to+back;
    }else
        board.enpassant=-1;
    history[historyIdx++]=h;
    board.empty = ~board.allOccupied;
    board.currentColor=(Color)(1-board.currentColor);
}
void unmakeMove(Board& board) {
    History h=history[--historyIdx];
    Move move=h.move;
    int from=fromSquare(move);
    int to=toSquare(move);
    int promo=promotionPiece(move);
    int flag=moveFlags(move);
    int piece=board.mailbox[to];
    int enemy=board.currentColor;
    int color=1-enemy;
    board.mailbox[from]=board.mailbox[to];
    board.mailbox[to]=EMPTY;
    U64 fromBB=(1ULL<<from);
    U64 toBB=(1ULL<<to);
    board.piece[color][piece]&=~toBB;
    board.piece[color][piece]|=fromBB;
    board.allOccupied&=~toBB;
    board.allOccupied|=fromBB;
    board.occupied[color]&=~toBB;
    board.occupied[color]|=fromBB;
    if (h.captureSquare!=-1) {
        board.mailbox[h.captureSquare]=h.captured;
        U64 back=(1ULL<<h.captureSquare);
        board.piece[enemy][h.captured]|=back;
        board.allOccupied|=back;
        board.occupied[enemy]|=back;
    }
    board.castlingRights=h.castlingRights;
    board.enpassant=h.prevEnpassant;
    board.halfMoveClock=h.halfMoveClock;
    if (board.mailbox[from]==KING)
        board.kingPos[color]=from;
    if (flag&(FLAG_CASTLE)) {
        if (to>from) {
            int end=7;
            if (color) end=63;
            board.mailbox[end]=ROOK;
            board.mailbox[end-2]=EMPTY;
            U64 finBB=(1ULL<<end);
            U64 endBB=(1ULL<<(end-2));
            board.piece[color][ROOK]&=~endBB;
            board.piece[color][ROOK]|=finBB;
            board.allOccupied&=~endBB;
            board.allOccupied|=finBB;
            board.occupied[color]&=~endBB;
            board.occupied[color]|=finBB;
        }else {
            int end=0;
            if (color) end=56;
            board.mailbox[end]=ROOK;
            board.mailbox[end+3]=EMPTY;
            U64 finBB=(1ULL<<end);
            U64 endBB=(1ULL<<(end+3));
            board.piece[color][ROOK]&=~endBB;
            board.piece[color][ROOK]|=finBB;
            board.allOccupied&=~endBB;
            board.allOccupied|=finBB;
            board.occupied[color]&=~endBB;
            board.occupied[color]|=finBB;
        }
    }
    if (promo) {
        board.piece[color][piece]&=~fromBB;
        board.mailbox[from]=PAWN;
        board.piece[color][PAWN]|=fromBB;
    }
    board.empty=~board.allOccupied;
    board.currentColor=(Color)(1-board.currentColor);
}
void addMove(Board& board,int from,int to,int promo,bool castle,bool enpassant) {
    int flag=0;
    if (castle)
        flag|=FLAG_CASTLE;
    if (enpassant) {
        flag|=FLAG_EN_PASSANT;
        flag|=FLAG_CAPTURE;
    }
    if (promo)
        flag|=FLAG_PROMOTION;
    int pieceMoved=board.mailbox[from];
    int pieceTaken=board.mailbox[to];
    int color=board.currentColor;
    if (pieceTaken!=EMPTY) {
        flag|=FLAG_CAPTURE;
    }
    Move move=make_move(from,to,promo,flag);
    makeMove(board,move);

    if (!isAttacked(board,color,board.kingPos[color])) {
        moves[moveIdx++]=move;
    }
    unmakeMove(board);
}
constexpr U64 WSC = (1ULL<<5)+(1ULL<<6);
constexpr U64 WLC = (1ULL<<1)+(1ULL<<2)+(1ULL<<3);
constexpr U64 BSC = (1ULL<<61)+(1ULL<<62);
constexpr U64 BLC = (1ULL<<57)+(1ULL<<58)+(1ULL<<59);


void generateMoves(Board& board) {
    int color=board.currentColor;
    U64 notOwn=~board.occupied[color];

    U64 pawns=board.piece[color][PAWN];
    U64 singlePushPawns=pawnsAble2Push(pawns,board.empty,color);
    U64 doublePushPawns=pawnsAble2DblPush(pawns,board.empty,color);
    while (singlePushPawns) {
        int from=__builtin_ctzll(singlePushPawns);
        int forward=(color==WHITE? 8:-8);
        int promoRank=(color==WHITE? 7:0);
        int to=from+forward;
        singlePushPawns&=singlePushPawns-1;
        if (to/8==promoRank) {
            for (int i=1;i<=4;i++) {
                addMove(board,from,to,i,false,false);
            }
        }else {
            addMove(board,from,to,0,false,false);
        }
    }
    while (doublePushPawns) {
        int from=__builtin_ctzll(doublePushPawns);
        int forward=(color==WHITE? 16:-16);
        int to=from+forward;
        doublePushPawns&=doublePushPawns-1;
        addMove(board,from,to,0,false,false);
    }
    while (pawns) {
        int promoRank=(color==WHITE? 7:0);
        int from=__builtin_ctzll(pawns);
        pawns&=pawns-1;
        U64 moves = pawnAttacks[color][from] & (board.occupied[1-color]);
        while (moves) {
            int to=__builtin_ctzll(moves);
            moves&=moves-1;
            if (to/8==promoRank) {
                for (int i=1;i<=4;i++) {
                    addMove(board,from,to,i,false,false);
                }
            }else {
                addMove(board,from,to,0,false,false);
            }
        }
        if (board.enpassant!=-1) {
            U64 enpassant= pawnAttacks[color][from] & (1ULL<<board.enpassant);
            if (enpassant) {
                int to=__builtin_ctzll(enpassant);
                addMove(board,from,to,0,false,true);
            }
        }
    }

    U64 knights=board.piece[color][KNIGHT];
    while (knights) {
        int from=__builtin_ctzll(knights);
        knights&=knights-1;
        U64 moves = knightAttacks[from] & ~board.occupied[color];
        while (moves) {
            int to=__builtin_ctzll(moves);
            moves&=moves-1;
            addMove(board,from,to,0,false,false);
        }
    }

    U64 bishops=board.piece[color][BISHOP];
    while (bishops) {
        int from=__builtin_ctzll(bishops);
        bishops&=bishops-1;
        U64 moves = bishopAttacks(from,board.allOccupied) & notOwn;
        while (moves) {
            int to=__builtin_ctzll(moves);
            moves&=moves-1;
            addMove(board,from,to,0,false,false);
        }
    }

    U64 rooks=board.piece[color][ROOK];
    while (rooks) {
        int from=__builtin_ctzll(rooks);
        rooks&=rooks-1;
        U64 moves = rookAttacks(from,board.allOccupied) & notOwn;
        while (moves) {
            int to=__builtin_ctzll(moves);
            moves&=moves-1;
            addMove(board,from,to,0,false,false);
        }
    }

    U64 queens=board.piece[color][QUEEN];
    while (queens) {
        int from=__builtin_ctzll(queens);
        queens&=queens-1;
        U64 moves = (rookAttacks(from,board.allOccupied)|bishopAttacks(from,board.allOccupied)) & notOwn;
        while (moves) {
            int to=__builtin_ctzll(moves);
            moves&=moves-1;
            addMove(board,from,to,0,false,false);
        }
    }

    U64 king=board.piece[color][KING];
    if (king) {
        int from=__builtin_ctzll(king);
        U64 moves = kingAttacks[from]&(~board.occupied[color]);
        while (moves) {
            int to=__builtin_ctzll(moves);
            moves&=moves-1;
            addMove(board,from,to,0,false,false);
        }
        if (!isAttacked(board,color,from)) {
            if (color==WHITE) {
                if (board.castlingRights&(1<<0)) {
                    if (!(board.allOccupied&WSC)&&!isAttacked(board,WHITE,6)&&!isAttacked(board,WHITE,5))
                        addMove(board,4,6,0,true,false);
                }
                if (board.castlingRights&(1<<1)) {
                    if (!(board.allOccupied&WLC)&&!isAttacked(board,WHITE,3)&&!isAttacked(board,WHITE,2))
                        addMove(board,4,2,0,true,false);
                }
            }else {
                if (board.castlingRights&(1<<2)) {
                    if (!(board.allOccupied&BSC)&&!isAttacked(board,BLACK,61)&&!isAttacked(board,BLACK,62))
                        addMove(board,60,62,0,true,false);
                }
                if (board.castlingRights&(1<<3)) {
                    if (!(board.allOccupied&BLC)&&!isAttacked(board,BLACK,58)&&!isAttacked(board,BLACK,59))
                        addMove(board,60,58,0,true,false);
                }
            }
        }
    }
}
long long perft(Board& board, int depth) {
    if (depth == 0) {
        return 1;
    }

    // Store the starting index for moves at this level
    int initialMoveIdx = moveIdx;
    generateMoves(board);
    // Get the count of moves generated for this position
    int movesGenerated = moveIdx - initialMoveIdx;

    long long nodes = 0;

    for (int i = 0; i < movesGenerated; i++) {
        // Access the correct move from the global list
        makeMove(board, moves[initialMoveIdx + i]);
        nodes += perft(board, depth - 1);
        unmakeMove(board);
    }

    // IMPORTANT: Restore the global move index for the parent call
    moveIdx = initialMoveIdx;

    return nodes;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    initAttacks();
    initMagic();
    auto start=chrono::high_resolution_clock::now();
    Board board;
    long long ans=perft(board,6);
    cout << ans << '\n';
    auto end=chrono::high_resolution_clock::now();
    chrono::duration<double,milli> elapsed=end-start;
    long long speed=(long long)((double)ans/elapsed.count());
    cout << "Time Elapsed: " << elapsed.count() << "ms\n";
    cout << "Speed: " << speed*1000 << "NPS\n";
}