cmake_minimum_required(VERSION 3.26)
project(skeet_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -static")
add_compile_options(-mbmi2)

add_executable(skeet_engine main.cpp
        types.h
        PieceBoard.h
        PieceBoard.cpp
        bitboard.cpp
        bitboard.cpp
        bitboard.cpp
        test2.h
        StateBoard.cpp
        StateBoard.h
        ZorbistFenBoard.cpp
        ZorbistFenBoard.h
        MoveBoard.cpp
        MoveBoard.h
        movegen.h
        movegen.cpp
        Eval.cpp
        Eval.h
        Search.cpp
        Search.h
        UCI.cpp
        UCI.h
        TranpositionTable.cpp
        TranpositionTable.h
)
