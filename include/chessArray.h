#pragma once
#include <string>

#include "gameData.h"

using namespace std;

void initializeBoard(gameData &active)
{
    char initialBoard[board_size][board_size] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};
    bool initialTeam1[board_size][board_size] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}};
    bool initialTeam2[board_size][board_size] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}};
    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            active.posArray[i][j] = initialBoard[i][j];         // set the board piece array
            active.player1Array[i][j] = initialTeam1[i][j];      // set which pieces are the player
            active.player2Array[i][j] = initialTeam2[i][j];    // set which pieces are the computer
        }
    }
}

void winCheck(int &winVal, gameData &active)
{
    // assume both kings are dead
    bool player1King = false,
         player2King = false;

    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            if (active.player1Array[i][j] && active.posArray[i][j] == 'k')
            {
                player1King = true; // player 1 king is alive
            }
            if (active.player2Array[i][j] && active.posArray[i][j] == 'k')
            {
                player2King = true; // player 2 king is alive
            }
        }
    }

    if (player1King && player2King)
    {
        winVal = 0; // if they are both alive, continue
        return;
    }
    else if (!player1King)
    {
        winVal = 1; // if player 1 king is dead
        return;
    }
    else if (!player2King)
    {
        winVal = 2; // if player 2 king is dead
        return;
    }
}