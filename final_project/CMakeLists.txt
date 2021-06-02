cmake_minimum_required(VERSION 3.3)
# TODO: CHANGE THE NEXT LINE TO THE INCLUDE THE TITLE OF YOUR GAME:
set(GAME_EXE chess)

project(${GAME_EXE} CXX)
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
        src/game_config.cxx
        )

# TODO: PUT ADDITIONAL NON-MODEL (UI) .cxx FILES IN THIS LIST:
add_program(${GAME_EXE}
    src/main.cxx
    src/view.cxx
    src/controller.cxx
    ${MODEL_SRC})
target_link_libraries(${GAME_EXE} ge211)

add_test_program(model_test
    test/model_test.cxx
    ${MODEL_SRC})
target_link_libraries(model_test ge211)

# vim: ft=cmake
