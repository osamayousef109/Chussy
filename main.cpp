#include <bits/stdc++.h>
using namespace std;

enum Piece {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING,EMPTY};
enum Color {WHITE,BLACK};

struct Board {
    Color currentColor=WHITE;
    bool whiteShortCastle=true;
    bool whiteLongCastle=true;
    bool blackShortCastle=true;
    bool blackLongCastle=true;
    int piece[64] = {
        ROOK,  KNIGHT,BISHOP,QUEEN, KING,  BISHOP,KNIGHT,ROOK,
        PAWN,  PAWN,  PAWN,  PAWN,  PAWN,  PAWN,  PAWN,  PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        PAWN,  PAWN,  PAWN,  PAWN,  PAWN,  PAWN,  PAWN,  PAWN,
        ROOK,  KNIGHT,BISHOP,QUEEN, KING,  BISHOP,KNIGHT,ROOK
    };
    int color[64] = {
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
        WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
    };
};


struct Move {
    uint8_t from;
    uint8_t to;
    uint8_t promotion;
    uint8_t capture;
};
constexpr uint8_t FLAG_CAPTURE   = 1 << 0;
constexpr uint8_t FLAG_EN_PASSANT= 1 << 1;
constexpr uint8_t FLAG_CASTLE    = 1 << 2;
constexpr uint8_t FLAG_PROMOTION = 1 << 3;

vector<Move> moves;
inline Move make_move(int from, int to, int promo=0, uint8_t flags=0) {
    return Move{
        static_cast<uint8_t>(from),
        static_cast<uint8_t>(to),
        static_cast<uint8_t>(promo),
        static_cast<uint8_t>(flags|(promo?FLAG_PROMOTION:0))
    };
}
string toString(Piece c) {
    switch (c) {
        case PAWN:   return "PAWN";
        case KNIGHT: return "KNIGHT";
        case BISHOP:  return "BISHOP";
        case ROOK:   return "ROOK";
        case QUEEN: return "QUEEN";
        case KING:  return "KING";
        default:    return "EMPTY";
    }
}
string toString(Color c) {
    switch (c) {
        case WHITE:   return "WHITE";
        case BLACK: return "BLACK";
        default:    return "EMPTY";
    }
}

vector<vector<pair<int,int>>> directions = {
    {},
    { {1,2},{1,-2},{2,1},{2,-1},{-1,2},{-1,-2},{-2,1},{-2,-1} },
    { {1,1},{1,-1},{-1,1},{-1,-1} },
    { {1,0},{-1,0},{0,1},{0,-1} },
    { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} },
    { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} }
};
bool inBoard(int x,int y) {
    return (x<8&&x>=0&&y<8&&y>=0);
}
bool isSliding[]={false,false,true,true,true,false};
void makeMove(Board& board,int from,int to,bool enpassant,bool castle) {
    board.piece[to]=board.piece[from];
    board.piece[from]=EMPTY;
    board.color[to]=board.color[from];
    board.color[from]=EMPTY;
    if (enpassant) {
        int back=(board.currentColor==WHITE? -8:8);
        back+=to;
        board.piece[back]=EMPTY;
        board.color[back]=EMPTY;
    }
    if (castle) {
        if (to>from) {
            int end=7;
            if (board.currentColor==BLACK) end=63;
            board.piece[end]=EMPTY;
            board.piece[end-2]=ROOK;
            board.color[end]=EMPTY;
            board.color[end-2]=board.currentColor;
        }else {
            int end=0;
            if (board.currentColor==BLACK) end=56;
            board.piece[end]=EMPTY;
            board.piece[end+3]=ROOK;
            board.color[end]=EMPTY;
            board.color[end+3]=board.currentColor;
        }
    }
    board.currentColor=(Color)(1-board.currentColor);
}
void undoMove(Board& board,int from,int to,int p) {
    board.piece[from]=board.piece[to];
    board.piece[to]=p;
    board.currentColor=(Color)(1-board.currentColor);
}
bool inCheck(Board& board,Color color) {
    int r=-1;
    int c=-1;
    bool found=false;
    for (int row=0;row<8;row++) {
        for (int col=0;col<8;col++) {
            int i=8*row+col;
            if (board.piece[i]==KING&&board.color[i]==color) {
                r=row;
                c=col;
                found=true;
                break;
            }
        }
        if (found) break;
    }
    if (!found) return false;
    for (auto [dr,dc]:directions[KNIGHT]) {
        int nr=r+dr;
        int nc=c+dc;
        if (!inBoard(nr,nc)) continue;
        int i=8*nr+nc;
        if (board.piece[i]==KNIGHT&&board.color[i]!=color)
            return true;
    }
    for (auto [dr,dc]:directions[ROOK]) {
        for (int step=1;step<=7;step++) {
            int nr=r+step*dr;
            int nc=c+step*dc;
            if (!inBoard(nr,nc)) break;
            int i=8*nr+nc;
            if (board.piece[i]==EMPTY) continue;
            if ((board.piece[i]==ROOK||board.piece[i]==QUEEN)&&board.color[i]!=color)
                return true;
            break;
        }
    }
    for (auto [dr,dc]:directions[BISHOP]) {
        for (int step=1;step<=7;step++) {
            int nr=r+step*dr;
            int nc=c+step*dc;
            if (!inBoard(nr,nc)) break;
            int i=8*nr+nc;
            if (board.piece[i]==EMPTY) continue;
            if ((board.piece[i]==BISHOP||board.piece[i]==QUEEN)&&board.color[i]!=color)
                return true;
            break;
        }
    }
    int forward = (color==WHITE ? -1 : 1);
    for (int dc : {-1, 1}) {
        int nr = r+forward, nc = c+dc;
        if (!inBoard(nr,nc)) continue;
        int i = 8*nr + nc;
        if (board.piece[i]==PAWN && board.color[i]!=color)
            return true;
    }

    for (auto [dr,dc] : directions[KING]) {
        int nr=r+dr, nc=c+dc;
        if (!inBoard(nr,nc)) continue;
        int i = 8*nr + nc;
        if (board.piece[i]==KING && board.color[i]!=color)
            return true;
    }
    return false;
}
void addMove(Board& board,int from,int to,bool castle,bool enpassant) {
    int p=board.piece[from];
    uint8_t flag=0;
    if (board.piece[to]!=EMPTY) flag|=FLAG_CAPTURE;
    if (castle) flag|=FLAG_CASTLE;
    if (enpassant) flag|=FLAG_EN_PASSANT;
    bool promo=p==PAWN && board.color[from]==WHITE && 63-to<=7 || p==PAWN && board.color[from]==BLACK && 7-to<=7;
    p=board.piece[to];
    Color currentColor=board.currentColor;
    makeMove(board,from,to,enpassant,castle);

    if (!inCheck(board,currentColor)) {
        if (promo) {
            for (int i=1;i<=4;i++) {
                moves.push_back(make_move(from,to,i,flag));
            }
        }else {
            moves.push_back(make_move(from,to,0,flag));
        }
    }

    undoMove(board,from,to,p);
}
void moveGenerator(Board& board) {
    for (int r=0;r<8;r++) {
        for (int c=0;c<8;c++) {
            int i=r*8+c;
            if (board.piece[i]==EMPTY) continue;
            if (board.color[i]==board.currentColor) {
                int piece=board.piece[i];
                if (piece!=PAWN) {
                    for (auto [dr,dc]:directions[piece]) {
                        for (int step=1;step<=(isSliding[piece]?7:1);step++) {
                            int nr=r+step*dr;
                            int nc=r+step*dc;
                            if (!inBoard(nr,nc)) break;
                            if ()
                        }
                    }
                }
            }
        }
    }
}
int main() {
    Board board;
}