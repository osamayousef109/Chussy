cmake_minimum_required(VERSION 3.30)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(ChessEngine main.cpp
        types.h
        bitboards.cpp
        bitboards.h
        magic.h
        attacks.cpp
        attacks.h
        move.h
        movegen.h
        test.cpp
        test.h
        TT.cpp
        TT.h
        search.h
        evaluation.h
        play.h
)