/*
nChesses

So you want to play chess, huh?
No?
You just want to read my code?
Okay, to each his own i guess.

Written and tested by Tom McCloskey
*/

#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <locale.h>

#include "gameData.h"     //gameData struct
#include "chessDisplay.h" //ncurses functions
#include "chessMoves.h"   //clusterfuck of chess moves
#include "chessArray.h"   //chess game manipulations

using namespace std;

int main()
{
    setlocale(LC_CTYPE, ""); // UTF8 ┌(▀Ĺ̯▀)┐

    initializeDisplay(); // ncurses setup

    gameData active; // create the active gameData

    initializeBoard(active); // fill the active gameData

    int row, col;
    getmaxyx(stdscr, row, col);                        // get the screen sized, used for positioning
    int xpos = (col / 2) - (width / 2),                // center x
        ypos = (row / 2) - (height / 2) - 1,           // center y
        currentMoveX = (col / 2) - 40,
        currentMoveY = (row / 2) - 3,
        activePlayerX = (col / 2) + 25,
        activePlayerY = (row / 2) - 2;
    WINDOW *board = newwin(height, width, ypos, xpos); // create the window for the board
    WINDOW *currentMove = newwin(3, 14, currentMoveY, currentMoveX);        // create the window for currentMove
    WINDOW *activePlayer = newwin(1, 10, 11, activePlayerX);
    drawCoord(ypos, xpos);                             // draws coordinate
    drawControls(row, col);                                    // draws controls
    refresh();                                         // refresh the screen

    int moveStep = 0,                // position in the move cycle
        piecePosition[2] = {-1, -1}, // cursor position
        movePosition[2] = {-1, -1},  // cursor past position
        winVal = 0,                  // win value 1 = you lose 2 = you win
        key,                         // input variable
        index = 0;                   // computers move index
    bool playing = true;             // which player is active

    timeout(10); // set timeout to non-blocking

    do
    {
        drawBoard(board, active, playing, piecePosition, movePosition);     // draw / redraw the board for each loop
        moveWindow(currentMove, active, piecePosition, movePosition);       // draw / redraw the currentPos window for each loop
        drawPlayer(activePlayer, playing);                                  // draw / redraw current player status
        key = getch();                                                      // wait for timeout or block
        timeout(-1);                                                        // set timeout to blocking
        move(active, key, moveStep, playing, piecePosition, movePosition);  // everything player move related
        winCheck(winVal, active);
    } while (winVal == 0);

    wrefresh(board);

    drawWinVal(activePlayer, winVal);
    timeout(-1);
    getch();

    endwin();
    return 0;
}
