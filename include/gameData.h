#ifndef GAMEDATA_H
#define GAMEDATA_H

const int board_size = 8;

const char coord[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

bool moveArray[board_size][board_size];

struct gameData {
    char posArray[board_size][board_size];
    bool player1Array[board_size][board_size];
    bool player2Array[board_size][board_size];
};

#endif