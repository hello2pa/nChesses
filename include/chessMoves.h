#pragma once
#include <string>

#include "gameData.h"

using namespace std;

// reset the possible move array
void initializeMoveArray()
{
    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            moveArray[i][j] = false; // set all to false
        }
    }
}

// used for controlling the selection cursor
// checks adjacent positions in a direction
// then searches in that direction with increasing width
void selection(bool boolArray[board_size][board_size], int selectPos[2], int iModifier, int jModifier)
{
    int i = selectPos[0],
        j = selectPos[1];

    if (boolArray[i + iModifier][j + jModifier] // check the adjacent locations first
        && j + jModifier < board_size && i + iModifier < board_size && j + jModifier > -1 && i + iModifier > -1)
    {
        selectPos[0] = i + iModifier;
        selectPos[1] = j + jModifier;
        return;
    }
    else if (boolArray[i + iModifier + jModifier][j + jModifier + iModifier] // diagonal check 1
             && !boolArray[i + jModifier][j + iModifier]                     // make sure the diagonal isn't square with another possible spot
             && i + iModifier + jModifier < board_size && j + jModifier + iModifier < board_size && i + iModifier + jModifier > -1 && j + jModifier + iModifier > -1)
    {
        selectPos[0] = i + iModifier + jModifier;
        selectPos[1] = j + jModifier + iModifier;
        return;
    }
    else if (boolArray[i + iModifier - jModifier][j + jModifier - iModifier] // diagonal check 2
             && !boolArray[i - jModifier][j - iModifier]                     // make sure the diagonal isn't square with another possible spot
             && i + iModifier - jModifier < board_size && j + jModifier - iModifier < board_size && i + iModifier - jModifier > -1 && j + jModifier - iModifier > -1)
    {
        selectPos[0] = i + iModifier - jModifier;
        selectPos[1] = j + jModifier - iModifier;
        return;
    }
    else // otherwise, begin the search
    {
        int search = 0; // search width

        while (search < board_size)
        {
            i = selectPos[0]; // reset original position for each loop
            j = selectPos[1];
            do
            {
                i += iModifier; // check the length of the board
                j += jModifier; // one by one

                int iSearch = search * abs(jModifier), // search width in the direction that
                    jSearch = search * abs(iModifier); // is not being pressed

                if (boolArray[i + iSearch][j + jSearch] // check + search
                    && i + iSearch < board_size && j + jSearch < board_size && i + iSearch > -1 && j + jSearch > -1)
                {
                    selectPos[0] = i + iSearch; // set new selection
                    selectPos[1] = j + jSearch;
                    return;
                }
                if (boolArray[i - iSearch][j - jSearch] // check - search
                    && i - iSearch < board_size && j - jSearch < board_size && i - iSearch > -1 && j - jSearch > -1)
                {
                    selectPos[0] = i - iSearch; // set new selection
                    selectPos[1] = j - jSearch;
                    return;
                }

            } while (i + iModifier > -1 && i + iModifier < board_size && j + jModifier > -1 && j + jModifier < board_size); // loop until i or j exceeds board limit

            search++; // then increase search width and go again
        }
    }
}

// set the moveArray with the possible moves
void possibleMoves(gameData &active, bool activeArray[board_size][board_size], bool enemyArray[board_size][board_size], bool playing, int currentPos[2], int nextPos[2], int &moveStep)
{
    initializeMoveArray(); // reset move array

    int i = currentPos[0], // set piece position
        j = currentPos[1];

    int directionArray[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    switch (active.posArray[i][j])
    {
    case 'p':
    { // pawn moveset
        if (playing) // bottom player
        {                          
            if (!activeArray[i - 1][j]) // if not a teammate directly in front
            {
                moveArray[i - 1][j] = true; // add one forward to array

                if (i == 6                    // and if at the starting position
                    && !activeArray[i - 2][j] // and not a teammate two spaces ahead
                    && !enemyArray[i - 1][j]) // and not an enemy directly in front
                {
                    moveArray[i - 2][j] = true; // add two forward to array
                }
            }

            if (enemyArray[i - 1][j - 1] // if an enemy is diagonal
                && j > 0)
            {
                moveArray[i - 1][j - 1] = true; // add diagonal to array
            }

            if (enemyArray[i - 1][j + 1] // same thing other diagonal
                && j + 1 < board_size)
            {
                moveArray[i - 1][j + 1] = true; // add other diagonal
            }
        } else // top player
        {
            if (!activeArray[i + 1][j]) // if not a teammate directly in front
            {
                moveArray[i + 1][j] = true; // add one forward to array

                if (i == 1                    // and if at the starting position
                    && !activeArray[i + 2][j] // and not a teammate two spaces ahead
                    && !enemyArray[i + 1][j]) // and not an enemy directly in front
                {
                    moveArray[i + 2][j] = true; // add two forward to array
                }
            }

            if (enemyArray[i + 1][j - 1] // if an enemy is diagonal
                && j > 0)
            {
                moveArray[i + 1][j - 1] = true; // add diagonal to array
            }

            if (enemyArray[i + 1][j + 1] // same thing other diagonal
                && j + 1 < board_size)
            {
                moveArray[i + 1][j + 1] = true; // add other diagonal
            }
        }

        break;
    }
    case 'b':
    { // bishop moveset

        for (int dir1 = 0; dir1 < 2; dir1++)     // 2x2 array for diagonals
        {                                        // {-1, -1} {-1, 1}
            for (int dir2 = 0; dir2 < 2; dir2++) //  {1, -1}  {1, 1}
            {
                int distance = 1;

                while (distance < board_size) // for each direction, increase distance until break
                {
                    // break before adding the position if there is a teammate on the position or if the position is off the board
                    if (activeArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance] || i + directionArray[dir1][0] * distance < 0 || i + directionArray[dir1][0] * distance > 7 || j + directionArray[dir2][0] * distance < 0 || j + directionArray[dir2][0] * distance > 7)
                    {
                        break;
                    }

                    // add position
                    moveArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance] = true;

                    // break after adding if the position is an enemy
                    if (enemyArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance])
                    {
                        break;
                    }

                    distance++;
                }
            }
        }

        break;
    }
    case 'r':
    { // rook moveset

        for (int dir1 = 0; dir1 < 4; dir1++) // map i and j to all possible directions
        {
            int distance = 1;

            while (distance < board_size) // for each direction, increase distance until break
            {
                // break before adding the position if there is a teammate on the position or if the position is off the board
                if (activeArray[i + directionArray[dir1][0] * distance][j + directionArray[dir1][1] * distance] || i + directionArray[dir1][0] * distance < 0 || i + directionArray[dir1][0] * distance > 7 || j + directionArray[dir1][1] * distance < 0 || j + directionArray[dir1][1] * distance > 7)
                {
                    break;
                }

                // add position
                moveArray[i + directionArray[dir1][0] * distance][j + directionArray[dir1][1] * distance] = true;

                // break after adding if the position is an enemy
                if (enemyArray[i + directionArray[dir1][0] * distance][j + directionArray[dir1][1] * distance])
                {
                    break;
                }

                distance++;
            }
        }

        break;
    }
    case 'n':
    { // knight moveset

        int distance = 2; // start at corners 2x2 away from piece

        for (int dir1 = 0; dir1 < 2; dir1++)     // 2x2 for diagonals
        {                                        // {-1, -1} {-1, 1}
            for (int dir2 = 0; dir2 < 2; dir2++) //  {1, -1} {1, 1}
            {
                // 2x2 away in each diagonal
                // 1 towards the piece in y and x results in
                // all possible 2x1 away from the piece

                // 1i and 2j
                if (!activeArray[i + directionArray[dir1][0] * distance - directionArray[dir1][0]][j + directionArray[dir2][0] * distance] && i + directionArray[dir1][0] * distance - directionArray[dir1][0] > -1 && i + directionArray[dir1][0] * distance - directionArray[dir1][0] < board_size && j + directionArray[dir2][0] * distance > -1 && j + directionArray[dir2][0] * distance < board_size)
                {
                    moveArray[i + directionArray[dir1][0] * distance - directionArray[dir1][0]][j + directionArray[dir2][0] * distance] = true;
                }
                // 2i and 1j
                if (!activeArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance - directionArray[dir2][0]] && i + directionArray[dir1][0] * distance > -1 && i + directionArray[dir1][0] * distance < board_size && j + directionArray[dir2][0] * distance - directionArray[dir2][0] > -1 && j + directionArray[dir2][0] * distance - directionArray[dir2][0] < board_size)
                {
                    moveArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance - directionArray[dir2][0]] = true;
                }
            }
        }

        break;
    }
    case 'k':
    { // king moveset

        // 4 cardinal directions
        for (int dir1 = 0; dir1 < 4; dir1++)
        {
            if (!activeArray[i + directionArray[dir1][0]][j + directionArray[dir1][1]] &&
            i + directionArray[dir1][0] >= 0 && i + directionArray[dir1][0] < board_size &&
            j + directionArray[dir1][1] >= 0 && j + directionArray[dir1][1] < board_size)
            {
                moveArray[i + directionArray[dir1][0]][j + directionArray[dir1][1]] = true;
            }
        }

        // 4 diagonals
        for (int dir1 = 0; dir1 < 2; dir1++)
        {
            for (int dir2 = 0; dir2 < 2; dir2++)
            {
                if (!activeArray[i + directionArray[dir1][0]][j + directionArray[dir2][0]] &&
                i + directionArray[dir1][0] >= 0 && i + directionArray[dir1][0] < board_size &&
                j + directionArray[dir2][0] >=0 && j + directionArray[dir2][0] < board_size)
                {
                    moveArray[i + directionArray[dir1][0]][j + directionArray[dir2][0]] = true;
                }
            }
        }

        break;
    }
    case 'q':
    { // queen moveset

        // copied from bishop
        for (int dir1 = 0; dir1 < 2; dir1++)
        {
            for (int dir2 = 0; dir2 < 2; dir2++)
            {
                int distance = 1;

                while (distance < board_size)
                {
                    if (activeArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance] || i + directionArray[dir1][0] * distance < 0 || i + directionArray[dir1][0] * distance > 7 || j + directionArray[dir2][0] * distance < 0 || j + directionArray[dir2][0] * distance > 7)
                    {
                        break;
                    }

                    moveArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance] = true;

                    if (enemyArray[i + directionArray[dir1][0] * distance][j + directionArray[dir2][0] * distance])
                    {
                        break;
                    }

                    distance++;
                }
            }
        }

        // copied from rook
        for (int dir1 = 0; dir1 < 4; dir1++)
        {
            int distance = 1;

            while (distance < board_size)
            {
                if (activeArray[i + directionArray[dir1][0] * distance][j + directionArray[dir1][1] * distance] || i + directionArray[dir1][0] * distance < 0 || i + directionArray[dir1][0] * distance > 7 || j + directionArray[dir1][1] * distance < 0 || j + directionArray[dir1][1] * distance > 7)
                {
                    break;
                }

                moveArray[i + directionArray[dir1][0] * distance][j + directionArray[dir1][1] * distance] = true;

                if (enemyArray[i + directionArray[dir1][0] * distance][j + directionArray[dir1][1] * distance])
                {
                    break;
                }

                distance++;
            }
        }

        break;
    }
    }
}

void move(gameData &active, int keyPress, int &moveStep, bool &playing, int currentPos[2], int nextPos[2])
{
    // set active array for player and enemy based on turn
    bool activeArray[board_size][board_size], enemyArray[board_size][board_size];
    for (int m = 0; m < board_size; m++)
    {
        for (int n = 0; n < board_size; n++)
        {
            if (playing)
            {
                activeArray[m][n] = active.player1Array[m][n];
                enemyArray[m][n] = active.player2Array[m][n];
            }
            else
            {
                activeArray[m][n] = active.player2Array[m][n];
                enemyArray[m][n] = active.player1Array[m][n];
            }
        }
    }

    // first scan top to bottom, left to right, set player as currentPos
    if (moveStep == 0)
    {
        for (int j = 0; j < board_size; j++)
        {
            for (int i = 0; i < board_size; i++)
            {
                if (activeArray[i][j])
                {
                    currentPos[0] = i;
                    currentPos[1] = j;

                    moveStep = 1; // increment move function
                    return;
                }
            }
        }
    }
    // second read player input for piece selection
    else if (moveStep == 1)
    {
        switch (keyPress)
        {
        case KEY_UP:
            selection(activeArray, currentPos, -1, 0);
            break;
        case KEY_DOWN:
            selection(activeArray, currentPos, 1, 0);
            break;
        case KEY_LEFT:
            selection(activeArray, currentPos, 0, -1);
            break;
        case KEY_RIGHT:
            selection(activeArray, currentPos, 0, 1);
            break;
        case 10:                                                           // enter on keypad
        case 32:                                                           // enter on keyboard
            possibleMoves(active, activeArray, enemyArray, playing, currentPos, nextPos, moveStep); // set the moveArray
            for (int j = 0; j < board_size; j++)
            {
                for (int i = 0; i < board_size; i++)
                {
                    if (moveArray[i][j])
                    {
                        nextPos[0] = i; // select arbitrary possible move to be the first selection
                        nextPos[1] = j;
                        moveStep = 2; // increment move function
                        return;
                    }
                }
            }
            return;
        }
    }
    // third read player input for move selection
    else if (moveStep == 2)
    {
        switch (keyPress)
        {
        case KEY_UP:
            selection(moveArray, nextPos, -1, 0);
            break;
        case KEY_DOWN:
            selection(moveArray, nextPos, 1, 0);
            break;
        case KEY_LEFT:
            selection(moveArray, nextPos, 0, -1);
            break;
        case KEY_RIGHT:
            selection(moveArray, nextPos, 0, 1);
            break;
        case KEY_BACKSPACE:  // read backspace
        case 127:            // multiple ways
        case '\b':           // for accessibility
            nextPos[0] = -1; // unset move selection
            nextPos[1] = -1;
            initializeMoveArray(); // reset move array
            moveStep = 1;          // decrement move function
            break;
        case 10:                                                                                     // enter on keypad
        case 32:                                                                                     // enter on keyboard
            if(playing)
            {
                active.player2Array[nextPos[0]][nextPos[1]] = false;                                    // kill p2 piece
                active.player1Array[currentPos[0]][currentPos[1]] = false;                                // piece 'from' location is no longer p1 controlled
                active.player1Array[nextPos[0]][nextPos[1]] = true;                                       // piece 'to' location is now p1 controlled
                active.posArray[nextPos[0]][nextPos[1]] = active.posArray[currentPos[0]][currentPos[1]]; // set the 'to' position to the selected piece value
                active.posArray[currentPos[0]][currentPos[1]] = '.';                                     // set the 'from' location to .
                if(active.posArray[nextPos[0]][nextPos[1]] == 'p' && 
                nextPos[0] == 0) active.posArray[nextPos[0]][nextPos[1]] = 'k';                          // pawn king rule check
                playing = false;
            }
            else
            {
                active.player1Array[nextPos[0]][nextPos[1]] = false;                                        // kill p1 piece
                active.player2Array[currentPos[0]][currentPos[1]] = false;                                // piece 'from' location is no longer p2 controlled
                active.player2Array[nextPos[0]][nextPos[1]] = true;                                       // piece 'to' location is now p2 controlled
                active.posArray[nextPos[0]][nextPos[1]] = active.posArray[currentPos[0]][currentPos[1]];   // set the 'to' position to the selected piece value
                active.posArray[currentPos[0]][currentPos[1]] = '.';                                       // set the 'from' location to .
                if(active.posArray[nextPos[0]][nextPos[1]] == 'p' && nextPos[0] == 7) active.posArray[nextPos[0]][nextPos[1]] = 'k';
                playing = true;
            }
           
            currentPos[0] = -1; // reset currentPos
            currentPos[1] = -1;
            nextPos[0] = -1; // reset nextPos
            nextPos[1] = -1;
            moveStep = 0;  // reset move
            initializeMoveArray(); // reset move array
            timeout(500); // delay
            return;
        }
    }
}