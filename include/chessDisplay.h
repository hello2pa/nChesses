#pragma once

#include <ncurses.h>
#include "gameData.h"

int width = 33,  // 8 columns, 3 wide space with grid lines (8 * (3 + 1) + 1)
    height = 17; // 8 rows, 1 tall space with grid lines (8 * (1 + 1) + 1)

void initializeDisplay()
{
    initscr();            // initialize the screen
    cbreak();             // read real time inputs, allow ctrl+c to exit
    keypad(stdscr, TRUE); // for reading arrow keys
    noecho();             // do not display the typed keys
    curs_set(0);          // BOO! invisible cursor

    start_color();                           // starts ncurses color. The ability to be colored should be checked,
    init_color(COLOR_MAGENTA, 1000, 1000, 1000); // but the game runs on color. Redefine COLOR_RED to be bright white
    init_color(COLOR_CYAN, 700,850,1000);    // redefine COLOR_CYAN to be light blue

    init_pair(1, COLOR_BLACK, COLOR_MAGENTA);   // black and white
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // black and gray
    init_pair(3, COLOR_BLACK, COLOR_GREEN); // black and green for selected tile p1
    init_pair(6, COLOR_BLACK, COLOR_RED); // black and red for selected tile p2
    init_pair(4, COLOR_BLACK, COLOR_BLUE); // black and blue for previous tile
    init_pair(5, COLOR_BLACK, COLOR_CYAN); // black and light blue for possible moves
    init_pair(7, COLOR_GREEN, COLOR_MAGENTA); // green on white for active p1
    init_pair(8, COLOR_RED, COLOR_MAGENTA); // red on white for active p2
    wbkgd(stdscr, COLOR_PAIR(1));           // set the main terminal to black and white
}

//draw / redraw the board
void drawBoard(WINDOW *board, gameData &active, bool playing, int currentPos[2], int nextPos[2])
{
    bool check = true;                      // checker pattern bool
    wbkgd(board, COLOR_PAIR(1));            // set board color first

    // draw pieces on the board second
    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            // convert array position to board position
            int xpos = j * 4 + 2,
                ypos = i * 2 + 1;

            if (active.player1Array[i][j])               // player 1 pieces are a outline char
            {
                switch (active.posArray[i][j])
                {
                case 'p':
                    mvwprintw(board, ypos, xpos, "♙");
                    break;
                case 'r':
                    mvwprintw(board, ypos, xpos, "♖");
                    break;
                case 'b':
                    mvwprintw(board, ypos, xpos, "♗");
                    break;
                case 'n':
                    mvwprintw(board, ypos, xpos, "♘");
                    break;
                case 'q':
                    mvwprintw(board, ypos, xpos, "♕");
                    break;
                case 'k':
                    mvwprintw(board, ypos, xpos, "♔");
                }
            }
            else if (active.player2Array[i][j])         // player 2 pieces are a solid char
            {
                switch (active.posArray[i][j])
                {
                case 'p':
                    mvwprintw(board, ypos, xpos, "♟");
                    break;
                case 'r':
                    mvwprintw(board, ypos, xpos, "♜");
                    break;
                case 'b':
                    mvwprintw(board, ypos, xpos, "♝");
                    break;
                case 'n':
                    mvwprintw(board, ypos, xpos, "♞");
                    break;
                case 'q':
                    mvwprintw(board, ypos, xpos, "♛");
                    break;
                case 'k':
                    mvwprintw(board, ypos, xpos, "♚");
                }
            }
            else                                        // fill empty spaces with a blank
            {
                mvwprintw(board, ypos, xpos, " ");
            }
        }
    }

    // draw the grid lines third
    for (int i = 0; i < height; i += 2)
    {
        for (int j = 0; j < width; j += 4)
        {
            if (i == 0 && j == 0) // first vert and hori lines
            {
                mvwvline(board, i, j, 0, height);
                mvwhline(board, i, j, 0, width);
            }
            else if (i == 0) // along the top, draw vert lines and add T
            {
                mvwvline(board, i, j, 0, height);
                mvwaddch(board, i, j, ACS_TTEE);
            }
            else if (j == 0) // along the side, draw hori lines and add T
            {
                mvwhline(board, i, j, 0, width);
                mvwaddch(board, i, j, ACS_LTEE);
            }
            else if (i == height - 1) // along bottom add T
            {
                mvwaddch(board, i, j, ACS_BTEE);
            }
            else if (j == width - 1) // along right side add T
            {
                mvwaddch(board, i, j, ACS_RTEE);
            }
            else // everywhere else add +
            {
                mvwaddch(board, i, j, ACS_PLUS);
            }
        }
    }

    // add corners
    mvwaddch(board, 0, 0, ACS_ULCORNER);
    mvwaddch(board, height - 1, 0, ACS_LLCORNER);
    mvwaddch(board, 0, width - 1, ACS_URCORNER);
    mvwaddch(board, height - 1, width - 1, ACS_LRCORNER);

    // loops for recoloring the checkered pattern
    for (int i = 1; i < height; i++)
    {
        if (i % 2 != 0)
        {
            check = !check;
        }
        for (int j = 1; j < width; j += 4)
        {
            if (check)
            {
                mvwchgat(board, i, j, 4, A_ALTCHARSET, 2, NULL); // gray spaces
            }
            else
            {
                mvwchgat(board, i, j, 4, A_ALTCHARSET, 1, NULL); // white spaces
            }
            check = !check;
        }
    }
    
    for(int i = 0; i < board_size; i++)
    {
        for(int j = 0; j < board_size; j++)
        {
            if(moveArray[i][j])
            {
                mvwchgat(board, (i * 2) + 1, (j * 4) + 1, 4, A_ALTCHARSET, 5, NULL);
                mvwchgat(board, (i * 2) + 2, (j * 4) + 1, 4, A_ALTCHARSET, 5, NULL);
            }
        }
    }

    // color the nextPos selection blue
    mvwchgat(board, (nextPos[0] * 2) + 1, (nextPos[1] * 4) + 1, 4, A_ALTCHARSET, 4, NULL);
    mvwchgat(board, (nextPos[0] * 2) + 2, (nextPos[1] * 4) + 1, 4, A_ALTCHARSET, 4, NULL);

    // color the currentPos selection green
    if(playing)
    {
        mvwchgat(board, (currentPos[0] * 2) + 1, (currentPos[1] * 4) + 1, 4, A_ALTCHARSET, 3, NULL);
        mvwchgat(board, (currentPos[0] * 2) + 2, (currentPos[1] * 4) + 1, 4, A_ALTCHARSET, 3, NULL);
    }
    else
    {
        mvwchgat(board, (currentPos[0] * 2) + 1, (currentPos[1] * 4) + 1, 4, A_ALTCHARSET, 6, NULL);
        mvwchgat(board, (currentPos[0] * 2) + 2, (currentPos[1] * 4) + 1, 4, A_ALTCHARSET, 6, NULL);
    }
    

    wrefresh(board); // refresh the window
}

// display current move in a window
void moveWindow(WINDOW *currentMove, gameData &active, int currentPos[2], int nextPos[2])
{
    wbkgd(currentMove, COLOR_PAIR(2));  // background color gray
    box(currentMove, 0, 0);             // outline black

    mvwprintw(currentMove, 0, 1, "Current Move");   // print text along the top of the box

    if(currentPos[0] >= 0)  // if currentPos is active
    {
        mvwprintw(currentMove, 1, 3, "%c", active.posArray[currentPos[0]][currentPos[1]]); // current piece
        mvwprintw(currentMove, 1, 5, "%c", coord[currentPos[1]]); // current horizontal pos
        mvwprintw(currentMove, 1, 6, "%d", board_size - currentPos[0]); // current vertical pos
    }
    else
    {
        mvwprintw(currentMove, 1, 3, "    "); // clear currentPos
    }

    if(nextPos[0] >= 0)
    {
        mvwprintw(currentMove, 1, 7, "→"); // arrow for cool factor
        mvwprintw(currentMove, 1, 8, "%c", coord[nextPos[1]]); // next horizontal position
        mvwprintw(currentMove, 1, 9, "%d", board_size - nextPos[0]); // next vertical position
    }
    else
    {
        mvwprintw(currentMove, 1, 7, "   "); // clear nextPos
    }

    wrefresh(currentMove); // refresh the window
}

// draw active player label
void drawPlayer(WINDOW *activePlayer, bool playing)
{
    if(playing)
    {
       wbkgd(activePlayer, COLOR_PAIR(7));
        mvwprintw(activePlayer, 0, 1, "P1's Turn"); 
    }
    else
    {
        wbkgd(activePlayer, COLOR_PAIR(8));
        mvwprintw(activePlayer, 0, 1, "P2's Turn");
    }
    

    wrefresh(activePlayer);
}

// draw coordinate labels along the side of the board
void drawCoord(int y, int x) // y and x passed in from the top left of the board
{
    for(int i = 0; i < board_size; i++)
    {
        mvprintw(y + (i * 2) + 1, x - 1, "%d", board_size - i);
    }
    for(int j = 0; j < board_size; j++)
    {
        mvprintw(y - 1, x + (j * 4) + 2, "%c", coord[j]);
    }
}

//self explanatory
void drawControls(int row, int col)
{
    int posX = (col / 2) - 27,
    posY = (row / 2) + 10;
    mvprintw(posY, posX, "[←↑→↓] to move   [enter] to enter    [bckspc] to return");
}

void drawWinVal(WINDOW *activePlayer, int winVal)
{
    if(winVal == 1)
    {
        wbkgd(activePlayer, COLOR_PAIR(8));
        mvwprintw(activePlayer, 0, 1, "P2 Wins! ");
    }
    if(winVal == 2)
    {
        wbkgd(activePlayer, COLOR_PAIR(7));
        mvwprintw(activePlayer, 0, 1, "P1 Wins! "); 
    }
    wrefresh(activePlayer);
}
