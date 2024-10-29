cmake_minimum_required(VERSION 3.26)
project(chess110)

set(CMAKE_CXX_STANDARD 17)

add_executable(chess110 main.cpp
        Piece.cpp
        Piece.h
        SimpleBoard.cpp
        SimpleBoard.h
        GameBoard.cpp
        GameBoard.h
        Move.cpp
        Move.h
        types.h
)
