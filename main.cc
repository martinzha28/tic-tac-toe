/* 
Tic-Tac-Toe, Written by Martin Zhao


The objective is to mark a line of 3 squares in a horizontal, vertical, or diagonal line. 
The game can be played over a command prompt or a pipelined file. THe syntax of each line of input consists of a player, row, and column they wish to mark. For example "x 0 0".
The current program keeps running a best-of series until the program is aborted. After every game, the scoreboard is printed.
Finally, there is eror-checking for bad files, invalid moves (coordinates are out of scope, more/less input on each line, incorrect player move, square has already been taken, or invalid characters). The program will move onto the next game and return an exit code of 1 with any error. 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

void printBoard(char *board)
{
        for (int i = 0; i < 3; i++)
        {
                for (int j = 0; j < 3; j++)
                {
                        cout << "|" << *(board + i * 3 + j);
                }
                cout << "|" << endl;
        }
}

// If move is valid, changes board.
// If move is not valid, return false

bool validMove(char *board, string s1, bool move)
{
        string s2;
        int inputs = 0;
        istringstream ss1{s1};
        while (ss1 >> s2 && s2.length() == 1)
        {
                inputs++;
        }
        if (inputs == 3)
        {
                char player;
                char c_row;
                char c_col;
                istringstream ss2{s1};
                ss2 >> player;
                ss2 >> c_row;
                ss2 >> c_col;

                if (isdigit(c_row) && isdigit(c_col))
                {
                        int row = c_row - '0';
                        int col = c_col - '0';
                        int index = row * 3 + col;

                        if ((move == 0 && player == 'x') || (move == 1 && player == 'o'))
                        {
                                if ((0 <= row) && (row <= 2) && (0 <= col) && (col <= 2))
                                {
                                        if (*(board + index) == ' ')
                                        {
                                                *(board + index) = player;
                                                return 1;
                                        }
                                }
                        }
                }
        }
        return 0;
}

// 0 is tie
// 1 is x win
// 2 is o win
// 3 is game is not finished

int checkWin(char *board)
{
        if ((*(board + 0) == *(board + 4) && *(board + 0) == *(board + 8) && *(board + 0) == 'x') ||
                (*(board + 2) == *(board + 4) && *(board + 2) == *(board + 6) && *(board + 2) == 'x'))
        {
                return 1;
        }
        else if ((*(board + 0) == *(board + 4) && *(board + 0) == *(board + 8) && *(board + 0) == 'o') ||
                         (*(board + 2) == *(board + 4) && *(board + 2) == *(board + 6) && *(board + 2) == 'o'))
        {
                return 2;
        }

        for (int i = 0; i < 3; i++)
        {
                if ((*(board + i * 3) == *(board + i * 3 + 1) && *(board + i * 3) == *(board + i * 3 + 2) && *(board + i * 3) == 'x') ||
                        (*(board + i) == *(board + 1 * 3 + i) && *(board + i) ==*(board + 2 * 3 + i) && *(board + i) == 'x'))
                {
                        return 1;
                }
                else if ((*(board + i * 3) == *(board + i * 3 + 1) && *(board +i * 3) == *(board + i * 3 + 2) && *(board + i * 3) == 'o') ||
                                 (*(board + i) == *(board + 1 * 3 + i) && *(board + i) == *(board + 2 * 3 + i) && *(board + i) == 'o'))
                {
                        return 2;
                }
        }

        for (int i = 0; i < 9; i++)
        {
                if (*(board + i) == ' ')
                {
                        return 3;
                }
        }

        return 0;
}

int main(int argc, char *argv[])
{

        int xwins = 0;
        int owins = 0;
        int ties = 0;
        int abort = 0;

        bool move = 0; // False is X turn, True is O turn
        char *board = new char[9]();

        for (int i = 0; i < 10; i++)
        {
                *(board + i) = ' ';
        }

        if (argc == 1)
        {
                string s1;
                while (getline(cin, s1))
                {
                        if (validMove(board, s1, move))
                        {
                                printBoard(board);
                                int winState = checkWin(board);
                                move = !move;

                                if (winState == 1)
                                {
                                        xwins++;
                                        cout << "x won!" << endl;
                                        goto end;
                                }
                                else if (winState == 2)
                                {
                                        owins++;
                                        cout << "o won!" << endl;
                                        goto end;
                                }
                                else if (winState == 0)
                                {
                                        ties++;
                                        cout << "Tie" << endl;
                                        goto end;
                                }
                        }
                        else
                        {
                                abort++;
                                cerr << "Invalid move: " << s1 << endl;
                                goto end;
                        }
                }
                cerr << "Unfinished game" << endl;
                abort++;
        }
        else
        {
                for (int argi = 1; argi < argc; argi++)
                {

                        string s1;
                        ifstream infile{argv[argi]};

                        if (!infile.good())
                        {
                                abort++;
                                cerr << "Bad File: " << argv[argi] << endl;
                                goto nextIteration;
                        }

                        while (getline(infile, s1))
                        {

                                if (validMove(board, s1, move))
                                {
                                        printBoard(board);
                                        int winState = checkWin(board);
                                        move = !move;

                                        if (winState == 1)
                                        {
                                                xwins++;
                                                cout << "x won!" << endl;
                                                goto nextIteration;
                                        }
                                        else if (winState == 2)
                                        {
                                                owins++;
                                                cout << "o won!" << endl;
                                                goto nextIteration;
                                        }
                                        else if (winState == 0)
                                        {
                                                ties++;
                                                cout << "Tie" << endl;
                                                goto nextIteration;
                                        }
                                }
                                else
                                {
                                        abort++;
                                        cerr << "Invalid move: " << s1 << endl;
                                        goto nextIteration;
                                }
                        }
                        cerr << "Unfinished game" << endl;
                        abort++;
                nextIteration:
                {
                }
                }
        }

end:
{
}

        cout << left << setw(16) << "x Wins:" << xwins << endl;
        cout << left << setw(16) << "o Wins:" << owins << endl;
        cout << left << setw(16) << "Ties:" << ties << endl;
        cout << left << setw(16) << "Aborted Games:" << abort << endl;

        if (abort == 0)
        {
                delete[] board;
                return 0;
        }
        else
        {
                delete[] board;
                return 1;
        }
}
