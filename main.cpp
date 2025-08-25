
#include "attacks.h"
#include "bitboards.h"
#include "play.h"

int main() {
    initAttacks();
    initMagic();
    Board board;
    board.initZobrist();
    while(true) {
        play(board);
    }
}
