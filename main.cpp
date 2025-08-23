
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
    long long ans=perft(board,6);
    std::cout << ans << '\n';
    auto end= std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed=end-start;
    long long speed=(long long)((double)ans/elapsed.count());
    std::cout << "Time Elapsed: " << elapsed.count() << "ms\n";
    std::cout << "Speed: " << speed*1000 << "NPS\n";
}
