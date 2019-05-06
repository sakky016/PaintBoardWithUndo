#include <conio.h>
#include <iostream>
#include"paint_board.h"

using namespace std;

//---------------------------------------------------------------------------------
// MAIN function
//---------------------------------------------------------------------------------
int main()
{
    bool done = false;
    PaintBoard board(100, 23);

    do
    {
        board.DrawBoard();
        board.FetchInput();

        // Process function returns 'true' if terminate
        // button was pressed by user
        done = board.Process();
    } while (!done);

    printf("***********************************************************\n");
    printf("                        D O N E \n");
    printf("***********************************************************\n");

    getchar();
    return 0;
}