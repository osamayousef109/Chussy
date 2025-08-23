
#ifndef MOVE_H
#define MOVE_H
#include "types.h"
#include "bitboards.h"
inline void makeMove(Board& board,Move move) {
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
inline void unmakeMove(Board& board) {
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
inline void addMove(Board& board,int from,int to,int promo,bool castle,bool enpassant) {
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
#endif //MOVE_H
