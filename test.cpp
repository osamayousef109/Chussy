#include "test.h"
#include "movegen.h"
#include "move.h"
#include "TT.h"

long long perft(Board& board, int depth) {
    if (depth == 0) {
        return 1;
    }
    long long nodes = 0;
    if (probePerftTT(board.zobristKey,depth,nodes)) {
        return nodes;
    }
    int initialMoveIdx = moveIdx;
    generateMoves(board);
    int movesGenerated = moveIdx - initialMoveIdx;
    for (int i = 0; i < movesGenerated; i++) {
        makeMove(board, moves[initialMoveIdx + i]);
        nodes += perft(board, depth - 1);
        unmakeMove(board);
    }
    moveIdx = initialMoveIdx;
    storePerftTT(board.zobristKey,depth,nodes);
    return nodes;
}
