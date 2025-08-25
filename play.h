
#ifndef PLAY_H
#define PLAY_H
#include <iostream>
#include "search.h"
inline void play(Board& board) {
    int from;
    int to;
    std::cin >> from >> to;
    int start=moveIdx;
    generateMoves(board);
    for (int i=start;i<moveIdx;i++) {
        Move move=moves[i];
        int f,t;
        f=fromSquare(move);
        t=toSquare(move);
        if (f==from&&t==to) {
            makeMove(board,move);
            break;
        }
    }
    moveIdx=start;
    alphaBeta(board,-INF,INF,6,0);
    std::cout << fromSquare(bestMove) << " " << toSquare(bestMove) << std::endl;
    makeMove(board,bestMove);
}
#endif //PLAY_H