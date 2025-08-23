#include "test.h"
#include "movegen.h"
#include "move.h"
long long perft(Board& board, int depth) {
    if (depth == 0) {
        return 1;
    }
    int initialMoveIdx = moveIdx;
    generateMoves(board);
    int movesGenerated = moveIdx - initialMoveIdx;
    long long nodes = 0;
    for (int i = 0; i < movesGenerated; i++) {
        makeMove(board, moves[initialMoveIdx + i]);
        nodes += perft(board, depth - 1);
        unmakeMove(board);
    }
    moveIdx = initialMoveIdx;
    return nodes;
}