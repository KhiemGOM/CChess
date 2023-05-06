cmake_minimum_required(VERSION 3.24)
project(Chess)

set(CMAKE_CXX_STANDARD 23)

set(Sources board.cpp pieces.cpp)
set(Headers board.h pieces.h)

add_executable(Chess
    main.cpp
    pieces.cpp pieces.h
    board.cpp board.h
    king.cpp king.h
    queen.cpp queen.h
    bishop.cpp bishop.h
    pawn.cpp pawn.h
    rook.cpp rook.h
    knight.cpp knight.h
        move_result.cpp move_result.h menu.cpp menu.h menu.h input_menu.h input_menu.h choice_menu.h choice_menu.h multiple_choice_menu.h multiple_choice_menu.h text_menu.h)

target_include_directories(Chess PUBLIC .)