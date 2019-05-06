#ifndef _PAINT_BOARD_H_
#define _PAINT_BOARD_H_
#include<map>
#include<list>

using namespace std;

//---------------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------------
const int MAX_UNDO = 50;
const char BLANK = '.';
const char CURSOR = 'O';
const char PAINTED = '*';
const char PAINTED_CURSOR = 'X';

const int KEY_ARROW_CHAR1 = 224;
const int KEY_ARROW_UP = 72;
const int KEY_ARROW_DOWN = 80;
const int KEY_ARROW_LEFT = 75;
const int KEY_ARROW_RIGHT = 77;

const int KEY_SPACE = 32; // <space bar>
const int KEY_STOP = 120; // x
const int KEY_UNDO = 117; // u

//---------------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------------
typedef enum
{
    CMD_NONE,
    CMD_MOVE_UP,
    CMD_MOVE_DOWN,
    CMD_MOVE_LEFT,
    CMD_MOVE_RIGHT,
    CMD_PAINT,
    CMD_UNDO,
    CMD_STOP
}KeyCommands_en;

//---------------------------------------------------------------------------------
// PaintBoard class 
//---------------------------------------------------------------------------------
class PaintBoard
{
private:
    int m_width;
    int m_height;
    int m_cursorX;
    int m_cursorY;
    KeyCommands_en m_nextCommand;
    map<int, bool> m_paintedArea;
    list<map<int, bool>> m_paintedAreaSnapshots;

public:
    PaintBoard(int width, int height);
    ~PaintBoard();
    void DisplayInstructions();
    void DrawBoard();
    bool MoveLeft();
    bool MoveRight();
    bool MoveUp();
    bool MoveDown();
    void FetchInput();
    bool Process();
    void PaintAtXY(int x, int y);
    void MakeSnapshot();
    void Restore();
};

#endif
