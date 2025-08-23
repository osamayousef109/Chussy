
#include <ios>
#include <iostream>
#include <bits/chrono.h>

#include "attacks.h"
#include "bitboards.h"
#include "test.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    initAttacks();
    initMagic();
    auto start= std::chrono::high_resolution_clock::now();
    Board board;
    board.setFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");
    board.initZobrist();
    long long ans=perft(board,8);
    std::cout << ans << '\n';
    auto end= std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed=end-start;
    long long speed=(long long)((double)ans/elapsed.count());
    std::cout << "Time Elapsed: " << elapsed.count() << "ms\n";
    std::cout << "Speed: " << speed*1000 << "NPS\n";
}
