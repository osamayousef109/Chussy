#include <bits/stdc++.h>
using namespace std;

enum Piece {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING,EMPTY};
enum Color {WHITE,BLACK,NO_COLOR};

struct Move {
    uint8_t from;
    uint8_t to;
    uint8_t promotion;
    uint8_t flags;
};

struct History {
    Move move;
    Piece captured;
    uint8_t castlingRights;
    int prevEnpassant;
    int captureSquare;
    int halfMoveClock;
};

struct Board {
    Color currentColor=WHITE;
    uint8_t castlingRights=15;
    int enpassant;
    int kingPos[2]={4,60};
    int halfMoveClock;
    vector<History> history;
    int piece[64] = {
        ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK,
        PAWN,  PAWN,   PAWN,   PAWN,  PAWN,  PAWN,   PAWN,   PAWN,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY,
        PAWN,  PAWN,   PAWN,   PAWN,  PAWN,  PAWN,   PAWN,   PAWN,
        ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK
    };

    int color[64] = {
        WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
        WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
        NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
        NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
        NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
        NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK
    };
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
void makeMove(Board& board,Move& move) {
    History h;
    int piece=board.piece[move.from];
    h.captured=(Piece)board.piece[move.to];
    h.prevEnpassant=board.enpassant;
    h.captureSquare=move.to;
    if (!(move.flags&(FLAG_CAPTURE))) h.captureSquare=-1;
    h.move=move;
    h.castlingRights=board.castlingRights;
    h.halfMoveClock=board.halfMoveClock;
    board.piece[move.to]=board.piece[move.from];
    board.piece[move.from]=EMPTY;
    board.color[move.to]=board.color[move.from];
    board.color[move.from]=NO_COLOR;
    if (move.flags&(FLAG_EN_PASSANT)) {
        int back=(board.currentColor==WHITE? -8:8);
        back+=move.to;
        h.captured=(Piece)board.piece[back];
        board.piece[back]=EMPTY;
        board.color[back]=NO_COLOR;
        h.captureSquare=back;
    }
    if (move.flags&(FLAG_CASTLE)) {
        if (move.to>move.from) {
            int end=7;
            if (board.currentColor==BLACK) end=63;
            board.piece[end]=EMPTY;
            board.piece[end-2]=ROOK;
            board.color[end]=NO_COLOR;
            board.color[end-2]=board.currentColor;
        }else {
            int end=0;
            if (board.currentColor==BLACK) end=56;
            board.piece[end]=EMPTY;
            board.piece[end+3]=ROOK;
            board.color[end]=NO_COLOR;
            board.color[end+3]=board.currentColor;
        }
    }
    if (move.promotion) {
        board.piece[move.to]=move.promotion;
    }
    if (piece!=PAWN&&!(move.flags&(FLAG_CAPTURE)))
        board.halfMoveClock++;
    else
        board.halfMoveClock=0;
    if (piece==KING) {
        if (board.color[move.to]==WHITE)
            board.castlingRights&=12;
        else
            board.castlingRights&=3;
        board.kingPos[board.color[move.to]]=move.to;
    }

    if (move.from==0||move.to==0)
        board.castlingRights&=13;
    if (move.from==7||move.to==7)
        board.castlingRights&=14;
    if (move.from==56||move.to==56)
        board.castlingRights&=7;
    if (move.from==63||move.to==63)
        board.castlingRights&=11;


    if (piece==PAWN&&abs(move.to/8-move.from/8)==2) {
        int back=(board.currentColor==WHITE? -8:8);
        board.enpassant=move.to+back;
    }else
        board.enpassant=-1;
    board.history.push_back(h);
    board.currentColor=(Color)(1-board.currentColor);
}
void unmakeMove(Board& board) {
    History h=board.history.back();
    Move move=h.move;
    board.history.pop_back();
    board.piece[move.from]=board.piece[move.to];
    board.piece[move.to]=EMPTY;
    board.color[move.from]=board.color[move.to];
    board.color[move.to]=NO_COLOR;
    if (h.captureSquare!=-1) {
        board.piece[h.captureSquare]=h.captured;
        board.color[h.captureSquare]=1-board.color[move.from];
    }
    board.castlingRights=h.castlingRights;
    board.enpassant=h.prevEnpassant;
    board.halfMoveClock=h.halfMoveClock;
    if (board.piece[move.from]==KING)
        board.kingPos[board.color[move.from]]=move.from;
    int color=board.color[move.from];
    if (move.flags&(FLAG_CASTLE)) {
        if (move.to>move.from) {
            int end=7;
            if (color==BLACK) end=63;
            board.piece[end]=ROOK;
            board.piece[end-2]=EMPTY;
            board.color[end]=color;
            board.color[end-2]=NO_COLOR;
        }else {
            int end=0;
            if (color==BLACK) end=56;
            board.piece[end]=ROOK;
            board.piece[end+3]=EMPTY;
            board.color[end]=color;
            board.color[end+3]=NO_COLOR;
        }
    }
    if (move.promotion)
        board.piece[move.from]=PAWN;
    board.currentColor=(Color)(1-board.currentColor);
}
bool isAttacked(Board& board,Color color,int position) {
    int r=position/8;
    int c=position%8;
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
    int forward = (color==WHITE ? 1 : -1);
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
void addMove(Board& board,int from,int to,int promo,bool castle,bool enpassant) {
    int p=board.piece[from];
    uint8_t flag=0;
    if (board.piece[to]!=EMPTY) flag|=FLAG_CAPTURE;
    if (castle) flag|=FLAG_CASTLE;
    if (enpassant) {
        flag|=FLAG_EN_PASSANT;
        flag|=FLAG_CAPTURE;
    }
    Color currentColor=board.currentColor;
    Move move=make_move(from,to,promo,flag);
    makeMove(board,move);
    if (!isAttacked(board,currentColor,board.kingPos[currentColor])) {
        moves.push_back(move);
    }
    unmakeMove(board);
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
                            int nc=c+step*dc;
                            if (!inBoard(nr,nc)) break;
                            int j=nr*8+nc;
                            if (board.color[j]==NO_COLOR) {
                                addMove(board,i,j,0,false,false);
                            }
                            else{
                                if (board.color[j]!=board.currentColor)
                                    addMove(board,i,j,0,false,false);
                                break;
                            }
                        }
                    }
                }
                if (piece==KING) {
                    if (isAttacked(board,(Color)board.color[i],i)) continue;
                    if (board.color[i]==WHITE) {
                        if (board.castlingRights&(1<<0)) {
                            if (board.piece[6]==EMPTY&&board.piece[5]==EMPTY&&!isAttacked(board,WHITE,6)&&!isAttacked(board,WHITE,5))
                                addMove(board,4,6,0,true,false);
                        }
                        if (board.castlingRights&(1<<1)) {
                            if (board.piece[1]==EMPTY&&board.piece[2]==EMPTY&&board.piece[3]==EMPTY&&!isAttacked(board,WHITE,3)&&!isAttacked(board,WHITE,2))
                                addMove(board,4,2,0,true,false);
                        }
                    }else {
                        if (board.castlingRights&(1<<2)) {
                            if (board.piece[61]==EMPTY&&board.piece[62]==EMPTY&&!isAttacked(board,BLACK,61)&&!isAttacked(board,BLACK,62))
                                addMove(board,60,62,0,true,false);
                        }
                        if (board.castlingRights&(1<<3)) {
                            if (board.piece[57]==EMPTY&&board.piece[58]==EMPTY&&board.piece[59]==EMPTY&&!isAttacked(board,BLACK,58)&&!isAttacked(board,BLACK,59))
                                addMove(board,60,58,0,true,false);
                        }
                    }
                }
                if (piece==PAWN) {
                    int forward=(board.color[i]==WHITE? 1:-1);
                    int startRank=(board.color[i]==WHITE? 1:6);
                    int promoRank=(board.color[i]==WHITE? 7:0);
                    int nr=r+forward;
                    int nc=c;
                    if (inBoard(nr,nc)) {
                        int j=nr*8+nc;
                        if (board.piece[j]==EMPTY) {
                            if (nr==promoRank) {
                                for (int k=1;k<=4;k++) {
                                    addMove(board,i,j,k,false,false);
                                }
                            }else {
                                addMove(board,i,j,0,false,false);
                            }
                        }
                    }
                    if (r==startRank) {
                        nr=r+forward*2;
                        if (inBoard(nr,nc)) {
                            int j=nr*8+nc;
                            int nj=(nr-forward)*8+nc;
                            if (board.piece[j]==EMPTY&&board.piece[nj]==EMPTY) {
                                addMove(board,i,j,0,false,false);
                            }
                        }
                    }
                    for (int dc : {-1, 1}) {
                        nr = r+forward,nc=c+dc;
                        if (!inBoard(nr,nc)) continue;
                        int j = 8*nr + nc;
                        if (board.color[j]!=board.color[i]&&board.piece[j]!=EMPTY) {
                            if (nr==promoRank) {
                                for (int k=1;k<=4;k++) {
                                    addMove(board,i,j,k,false,false);
                                }
                            }else {
                                addMove(board,i,j,0,false,false);
                            }
                        }
                        if (board.enpassant==j)
                            addMove(board,i,j,0,false,true);
                    }
                }
            }
        }
    }
}
long long perft(Board& board, int depth) {
    // Base case: If we're at the desired depth, we count this position as one "leaf node".
    if (depth == 0) {
        return 1;
    }

    moves.clear(); // Clear the global moves vector for the new position
    moveGenerator(board);

    long long nodes = 0;

    // It's safer to copy the generated moves to a local variable
    // in case the global 'moves' vector is modified in deeper recursive calls.
    vector<Move> currentMoves = moves;

    for (Move& move : currentMoves) {
        makeMove(board, move);
        nodes += perft(board, depth - 1); // Recursively call for the next depth
        unmakeMove(board); // Backtrack to the original position
    }

    return nodes;
}
void setBoard(Board& board, const string& fen) {
    // 1. Clear the existing board state
    for (int i = 0; i < 64; ++i) {
        board.piece[i] = EMPTY;
        board.color[i] = NO_COLOR;
    }
    board.history.clear();
    board.castlingRights = 0;
    board.enpassant = -1;
    board.halfMoveClock = 0;

    // Use a stringstream for easy parsing
    stringstream ss(fen);
    string field;

    // Field 1: Piece Placement
    ss >> field;
    int rank = 7, file = 0;
    map<char, Piece> pieceMap = {
        {'p', PAWN}, {'n', KNIGHT}, {'b', BISHOP}, {'r', ROOK}, {'q', QUEEN}, {'k', KING}
    };

    for (char c : field) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            file += (c - '0');
        } else {
            int square = rank * 8 + file;
            Color pieceColor = isupper(c) ? WHITE : BLACK;
            Piece pieceType = pieceMap[tolower(c)];

            board.piece[square] = pieceType;
            board.color[square] = pieceColor;

            if (pieceType == KING) {
                board.kingPos[pieceColor] = square;
            }
            file++;
        }
    }

    // Field 2: Active Color
    ss >> field;
    board.currentColor = (field == "w") ? WHITE : BLACK;

    // Field 3: Castling Availability
    ss >> field;
    if (field != "-") {
        for (char c : field) {
            if (c == 'K') board.castlingRights |= (1 << 0);
            if (c == 'Q') board.castlingRights |= (1 << 1);
            if (c == 'k') board.castlingRights |= (1 << 2);
            if (c == 'q') board.castlingRights |= (1 << 3);
        }
    }

    // Field 4: En Passant Target Square
    ss >> field;
    if (field != "-") {
        int epFile = field[0] - 'a';
        int epRank = field[1] - '1';
        board.enpassant = epRank * 8 + epFile;
    } else {
        board.enpassant = -1;
    }

    // Field 5: Halfmove Clock
    if (ss >> field) {
        board.halfMoveClock = stoi(field);
    } else {
        board.halfMoveClock = 0;
    }

    // Field 6: Fullmove Number (not stored in your struct, so we can ignore it)
    // ss >> field;
}


int main() {
    Board board;


}