//
// Created by MSi on 8/25/2025.
//

#ifndef SEARCH_H
#define SEARCH_H
#include "evaluation.h"
#include "movegen.h"
inline Move bestMove=-1;
inline int alphaBeta(Board& board,int alpha,int beta,int depth,int ply) {
    if (depth==0)
        return eval(board,ply);
    int start=moveIdx;
    generateMoves(board);
    int bestValue=-INF;
    Move best=moves[start];
    for (int i=start;i<moveIdx;i++) {
        int from=fromSquare(moves[i]);
        int to=toSquare(moves[i]);
        if (from==58&&to==30) {
            int hi=1;
        }
        makeMove(board,moves[i]);
        int score=-alphaBeta(board,-beta,-alpha,depth-1,ply+1);
        unmakeMove(board);
        if (score>bestValue) {
            bestValue=score;
            best=moves[i];
        }
        if (score>alpha)
            alpha=score;
        if (alpha>=beta) {
            moveIdx=start;
            return score;
        }
    }
    if (ply==0)
        bestMove=best;
    if (start==moveIdx) {
        if (isAttacked(board,board.currentColor,board.kingPos[board.currentColor]))
            return -MATE+ply;
        return 0;
    }
    moveIdx=start;
    return bestValue;
}

#endif //SEARCH_H
