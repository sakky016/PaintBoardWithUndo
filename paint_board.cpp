#include<conio.h>
#include"paint_board.h"
#include<iostream>

//----------------------------------------------------------------------------------------------
// @name                            : PaintBoard
//
// @description                     : Constructor
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
PaintBoard::PaintBoard(int width, int height)
{
    m_width = width;
    m_height = height;
    m_cursorX = 0;
    m_cursorY = 0;
    m_nextCommand = CMD_NONE;
    MakeSnapshot();
}

//----------------------------------------------------------------------------------------------
// @name                            : ~PaintBoard
//
// @description                     : Destructor
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
PaintBoard::~PaintBoard()
{

}

//----------------------------------------------------------------------------------------------
// @name                            : DisplayInstructions
//
// @description                     : Display instructions on screen
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
void PaintBoard::DisplayInstructions()
{
    printf("arrow keys : To move UP, DOWN, LEFT, RIGHT\n");
    printf("<space>    : Toggles paint on current co-ordinate\n");
    printf("u          : Undo\n");
    printf("x          : Terminate\n");
}

//----------------------------------------------------------------------------------------------
// @name                            : DrawBoard
//
// @description                     : Creates the drawing board in the form of a matrix. The 
//                                    symbol to be displayed at each coordinate depends upon
//                                    cursor location, blank space and painted area.
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
void PaintBoard::DrawBoard()
{
    // Clear the screen
    system("cls");

    // Print the board
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            int index = j * m_height + i;
            if (i == m_cursorY && j == m_cursorX &&
                m_paintedArea[index] == true)
            {
                cout << PAINTED_CURSOR;
            }
            else if (i == m_cursorY && j == m_cursorX)
            {
                cout << CURSOR;
            }
            else if (m_paintedArea[index] == true)
            {
                cout << PAINTED;
            }
            else
            {
                cout << BLANK;
            }
        }
        // End of a row
        cout << endl;
    }

    // Display other info
    printf("\nx,y (%d, %d)\n", m_cursorX, m_cursorY);
    printf("Undo marks: %ld\n", m_paintedAreaSnapshots.size());
    DisplayInstructions();
}

//----------------------------------------------------------------------------------------------
// @name                            : MoveLeft
//
// @description                     : Move cursor to left
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
bool PaintBoard::MoveLeft()
{
    if (m_cursorX > 0)
    {
        m_cursorX--;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------
// @name                            : MoveRight
//
// @description                     : Move cursor to right
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
bool PaintBoard::MoveRight()
{
    if (m_cursorX < m_width - 1)
    {
        m_cursorX++;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------
// @name                            : MoveUp
//
// @description                     : Move cursor up
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
bool PaintBoard::MoveUp()
{
    if (m_cursorY > 0)
    {
        m_cursorY--;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------
// @name                            : MoveDown
//
// @description                     : Move cursor down
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
bool PaintBoard::MoveDown()
{
    if (m_cursorY < m_height - 1)
    {
        m_cursorY++;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------
// @name                            : FetchInput
//
// @description                     : Waits for fetching user input. Translates the given input
//                                    to a command and stores it. 2 types of inputs are processed
//                                    here. ch1 is the 1st character that user enters. This is
//                                    the input that he is feeding. But in case of arrow keys,
//                                    a sequence of 2 characters are received ch1 and ch2. ch1 
//                                    identifies that some arrow key was pressed, ch2 determines the 
//                                    specific arrow key pressed.
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
void PaintBoard::FetchInput()
{
    unsigned char ch1 = _getch();
    if (ch1 == KEY_ARROW_CHAR1)
    {
        // Some Arrow key was pressed, determine which?
        unsigned char ch2 = _getch();
        switch (ch2) 
        {
        case KEY_ARROW_UP:
            // code for arrow up
            m_nextCommand = CMD_MOVE_UP;
            cout << "KEY_ARROW_UP" << endl;
            break;
        case KEY_ARROW_DOWN:
            // code for arrow down
            m_nextCommand = CMD_MOVE_DOWN;
            cout << "KEY_ARROW_DOWN" << endl;
            break;
        case KEY_ARROW_LEFT:
            // code for arrow right
            m_nextCommand = CMD_MOVE_LEFT;
            cout << "KEY_ARROW_LEFT" << endl;
            break;
        case KEY_ARROW_RIGHT:
            // code for arrow left
            m_nextCommand = CMD_MOVE_RIGHT;
            cout << "KEY_ARROW_RIGHT" << endl;
            break;
        }
    }
    else
    {
        switch (ch1)
        {
        case KEY_SPACE:
            // Paint
            m_nextCommand = CMD_PAINT;
            cout << "KEY_SPACE" << endl;
            break;
        case KEY_STOP:
            // Stop
            m_nextCommand = CMD_STOP;
            cout << "KEY_STOP" << endl;
            break;
        case KEY_UNDO:
            // Undo
            m_nextCommand = CMD_UNDO;
            cout << "KEY_UNDO" << endl;
            break;
        }
    }
}

//----------------------------------------------------------------------------------------------
// @name                            : Process
//
// @description                     : Depending on the command present in m_nextCommand variable,
//                                    function is executed
//
// @returns                         : True, if processing is complete, i.e, user pressed terminate 
//                                    button. 
//                                    False, if application continues.
//----------------------------------------------------------------------------------------------
bool PaintBoard::Process()
{
    if (m_nextCommand != CMD_NONE)
    {
        switch (m_nextCommand)
        {
        case CMD_MOVE_UP:
            MoveUp();
            break;
        case CMD_MOVE_DOWN:
            MoveDown();
            break;
        case CMD_MOVE_LEFT:
            MoveLeft();
            break;
        case CMD_MOVE_RIGHT:
            MoveRight();
            break;
        case CMD_PAINT:
            PaintAtXY(m_cursorX, m_cursorY);
            break;
        case CMD_UNDO:
            Restore();
            break;
        case CMD_STOP:
            // Processing done. Indicate the calling function
            return true;
        }

        m_nextCommand = CMD_NONE;
    }

    return false;
}

//----------------------------------------------------------------------------------------------
// @name                            : PaintAtXY
//
// @description                     : Stores the location of the painted (PAINTED) character.
//                                    Additionally, it creates a snapshot of the current painted
//                                    area of this board. This will later be used for undo operation.
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
void PaintBoard::PaintAtXY(int x, int y)
{
    int index = x * m_height + y;

    if (m_paintedArea[index] == true)
    {
        m_paintedArea[index] = false;
    }
    else
    {
        m_paintedArea[index] = true;
    }

    MakeSnapshot();
}

//----------------------------------------------------------------------------------------------
// @name                            : MakeSnapshot
//
// @description                     : Creates a snapshot of the painted area and stores it in
//                                    a list. This list cannot grow beyond MAX_UNDO marks.
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
void PaintBoard::MakeSnapshot()
{
    if (m_paintedAreaSnapshots.size() >= MAX_UNDO)
    {
        m_paintedAreaSnapshots.pop_front();
    }

    m_paintedAreaSnapshots.push_back(m_paintedArea);
}

//----------------------------------------------------------------------------------------------
// @name                            : Restore
//
// @description                     : Restores the painted area information as per the previous
//                                    undo mark.
//
// @returns                         : Nothing
//----------------------------------------------------------------------------------------------
void PaintBoard::Restore()
{
    // This reverts the current painted area, because after a paint operation
    // current painted area and the latest snapshot in the m_paintedAreaSnapshots
    // are basically same.
    if (m_paintedAreaSnapshots.size())
    {
        m_paintedArea = m_paintedAreaSnapshots.back();
        m_paintedAreaSnapshots.pop_back();
    }

    // This reverts to the previous painted area.
    if (m_paintedAreaSnapshots.size())
    {
        m_paintedArea = m_paintedAreaSnapshots.back();
        m_paintedAreaSnapshots.pop_back();
    }

    // Once we revert, we need to create a snapshot of this 
    // operation as well.
    MakeSnapshot();
}