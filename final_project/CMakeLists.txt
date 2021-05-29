cmake_minimum_required(VERSION 3.3)
# TODO: CHANGE THE NEXT LINE TO THE INCLUDE THE TITLE OF YOUR GAME:
project(chess CXX)
include(.cs211/cmake/CMakeLists.txt)

# TODO: PUT ADDITIONAL MODEL .cxx FILES IN THIS LIST:
set(MODEL_SRC
        src/model.cxx
        src/player.cxx
        src/piece.cxx
        src/board.cxx
        src/piece_set.cxx
        src/position_set.cxx
        src/move.cxx
#        src/std_black.hxx
#        src/std_white.hxx
        )

#trying this out to include these as headers
#set(HEADERS
#        src/model.hxx
#        src/player.hxx
#        src/piece.hxx
#        src/board.hxx
#        src/piece_set.hxx
#        src/position_set.hxx
#        src/move.hxx
#        src/std_black.hxx
#        src/std_white.hxx
#        )

#adding the locations of all of my sprites
#set(SPRITES
#        rsrc/black_bishop.png
#        rsrc/black_king.png
#        rsrc/black_knight.png
#        rsrc/black_pawn.png
#        rsrc/black_queen.png
#        rsrc/black_rook.png
#        rsrc/white_bishop.png
#        rsrc/white_king.png
#        rsrc/white_knight.png
#        rsrc/white_pawn.png
#        rsrc/white_queen.png
#        rsrc/white_rook.png
#        )

# TODO: PUT ADDITIONAL NON-MODEL (UI) .cxx FILES IN THIS LIST:
add_program(main
    src/main.cxx
    src/view.cxx
    src/controller.cxx
    ${MODEL_SRC})#${SPRITES}) #${HEADERS})
target_link_libraries(main ge211)

add_test_program(model_test
    test/model_test.cxx
    ${MODEL_SRC})
target_link_libraries(model_test ge211)

# vim: ft=cmake
