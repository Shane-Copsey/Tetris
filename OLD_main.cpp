#include<iostream>
#include<windows.h>
#include"conio.h"
#include<time.h>
#include<thread>
#include<mutex>
#include<cmath>

std::mutex only_mutex;


//#define GAME_INPUT_DELAY 0  // in milliseconds
#define SCREEN_HEIGHT 21
#define SCREEN_WIDTH 23
// SCREEN is where all of the characters of the game are contained besides BORDERS and FILL_LEFT
#define INDENT_X 0
#define INDENT_Y 2
/*
    * INDENT_X and INDENT_Y determine the screen's position
    * relative to the top left of the console window
*/
#define BORDERS true
// BORDERS will determine if a border should be put above and below the SCREEN
#define FILL_LEFT false
/*
    * FILL_LEFT will determine if the empty space
    * on the left (from indenting the SCREEN)
    * should be filled or not
*/
#define GAME_HEIGHT 21  // must be equal to SCREEN_HEIGHT (above)
#define GAME_WIDTH 10  // if this is an odd number then modify X_BLOCK_START (below)
#define ABOVE_CEILING 1
/*
    * space above the display where blocks may be
    * it is also the number added to the Y value when accessing the screen or game unless it has to do with blocks
    * however it is added to the tetrominos' starting positions
*/
#define GAME_COORD_X 1
#define GAME_COORD_Y 0+ABOVE_CEILING
// GAME is the empty space where the Tetris blocks move down, collect, and clear
/*
    * GAME displaces from the bottom left of SCREEN
    * (the offset is the bottom left of the screen)
    * meaning that GAME_COORD_Y goes from the bottom of the screen up
    * instead of from the top of the console window and
    * GAME_COORD_X goes from the side of the screen in
    * instead of from the side of the console window
*/
#define NEXTBLOCK_BOX_HEIGHT 6
#define NEXTBLOCK_BOX_WIDTH 10 // This doesn't actually affect the width of the box
// The box will go to the end of SCREEN

// The following positions start from 0
#define X_INDENT_OFNEXTBLOCK_BOX 0
#define Y_POSITION_OFNEXTBLOCK_BOX 0+ABOVE_CEILING
#define X_INDENT_OFSTRINGS 1  // The indent of "level" "score" "lines"
#define X_INDENT_OFVALUES 1  // The indent of the values of level, score, and lines
#define Y_POSITION_OFLEVEL 7+ABOVE_CEILING
#define Y_POSITION_OFLEVEL_NUM 8+ABOVE_CEILING
#define Y_POSITION_OFSCORE 11+ABOVE_CEILING
#define Y_POSITION_OFSCORE_NUM 12+ABOVE_CEILING
#define Y_POSITION_OFLINES 15+ABOVE_CEILING
#define Y_POSITION_OFLINES_NUM 16+ABOVE_CEILING
#define Y_PAUSE 19+ABOVE_CEILING
// To change the looks of the box and its dimensions go to the reset_array function
// To change the strings of level, score, and lines, go to the reset_array function
#define CLEAR_BELOW_NEXTBLOCK_BOX true  // true to clear below the next block box, false to clear above it
#define NEEDED_SPACE_FORSTRINGS_ANDVALUES 8  // keep in mind that the reset_array function
// needs addressing if you want to actually change how everything is displayed
const short border_rightof_game=GAME_COORD_X+GAME_WIDTH;
const short displacement_side=border_rightof_game+1;
/*
    * the above variable is the displacement into the side part of
    * the SCREEN past the GAME window where the level, score, and lines
    * are listed along with the next block box
*/
const short Y_LAST_ELEMENT_OFNEXTBLOCK_BOX=NEXTBLOCK_BOX_HEIGHT-1;
const short X_LAST_ELEMENT_OFNEXTBLOCK_BOX=NEXTBLOCK_BOX_WIDTH-1; // See NEXTBLOCK_BOX_WIDTH above
// This is not actually the last column element of the box
// The last column element is one index before the border to the right
//#define Y_VALUE 0  // just know that the Y value of a point is the first index of said point
//#define X_VALUE 1  // and the X value of a point is the second index of said point
#define BORDER_CHAR char(2)
#define BLOCK_CHAR char(219)
#define BACKGROUND_CHAR ' '
const short X_POSITION_OFNEXTBLOCK_BOX=displacement_side+X_INDENT_OFNEXTBLOCK_BOX;
const short X_POSITION_OFSTRINGS=displacement_side+X_INDENT_OFSTRINGS;
const short X_POSITION_OFVALUES=displacement_side+X_INDENT_OFVALUES;
const short Y_BLOCK_START=0+ABOVE_CEILING;
const short X_BLOCK_START=GAME_WIDTH/2-1+GAME_COORD_X;
#define ZIGGY_LEFT {{Y_BLOCK_START+1,X_BLOCK_START},{Y_BLOCK_START,X_BLOCK_START-1},{Y_BLOCK_START,X_BLOCK_START},{Y_BLOCK_START+1,X_BLOCK_START+1}}
#define ZIGGY_RIGHT {{Y_BLOCK_START,X_BLOCK_START},{Y_BLOCK_START,X_BLOCK_START+1},{Y_BLOCK_START+1,X_BLOCK_START},{Y_BLOCK_START+1,X_BLOCK_START-1}}
#define L_LEFT {{Y_BLOCK_START,X_BLOCK_START},{Y_BLOCK_START,X_BLOCK_START-1},{Y_BLOCK_START,X_BLOCK_START+1},{Y_BLOCK_START+1,X_BLOCK_START-1}}
#define L_RIGHT {{Y_BLOCK_START,X_BLOCK_START},{Y_BLOCK_START,X_BLOCK_START-1},{Y_BLOCK_START,X_BLOCK_START+1},{Y_BLOCK_START+1,X_BLOCK_START+1}}
#define LONG_FOUR {{Y_BLOCK_START,X_BLOCK_START},{Y_BLOCK_START,X_BLOCK_START-1},{Y_BLOCK_START,X_BLOCK_START+1},{Y_BLOCK_START,X_BLOCK_START+2}}
#define THREEWAY_INTERSECTION {{Y_BLOCK_START,X_BLOCK_START},{Y_BLOCK_START,X_BLOCK_START+1},{Y_BLOCK_START,X_BLOCK_START-1},{Y_BLOCK_START+1,X_BLOCK_START}}
#define SQUARE {{Y_BLOCK_START,X_BLOCK_START},{Y_BLOCK_START,X_BLOCK_START+1},{Y_BLOCK_START+1,X_BLOCK_START},{Y_BLOCK_START+1,X_BLOCK_START+1}}
#define MIN_LEVEL -32768
#define MAX_LEVEL 32767
#define FIRST_LEVEL 1
#define LAST_LEVEL 10
/// warning: levels should start at 1, if they do not you must adjust the formula where FIRST_LEVEL and LAST_LEVEL are used
/// if the levels start at 0, then add 1 where FIRST_LEVEL and LAST_LEVEL appear
#define POINTS_AWARDED_WHEN_CLEARED {40,100,300,1200}
#define POINTS_AWARDED_WHEN_DROPPED {1,2}
#define STARTING_SPEED 500  // in milliseconds
#define LAST_LEVEL_SPEED 100  // in milliseconds
#define FORMULA_DISPLACEMENT_CONST 3.688836002
#define FORMULA_BASE_CONST .5704466152
#define DIFFICULTY_INDEX 10
const short range=STARTING_SPEED-LAST_LEVEL_SPEED;
const short last_level_speed_minus_one=LAST_LEVEL_SPEED-1;
const double difficulty_increase=DIFFICULTY_INDEX/LAST_LEVEL;
const double formula_const_a=(FORMULA_DISPLACEMENT_CONST*range);
const short start_of_screen=INDENT_Y-ABOVE_CEILING+BORDERS;  // Y var
const COORD game_resting_position={0,start_of_screen+SCREEN_HEIGHT+BORDERS};
char move_left[2]={'a','A'};
char move_right[2]={'d','D'};
char rotate_cw[4]={'w','W','x','X'};
char rotate_ccw[2]={'z','Z'};
char soft_drop[2]={'s','S'};
char hard_drop[3]={'c','C',' '};
#define DEFAULT_COLOR 0x07
const WORD tetromino_colors[7] = {0x0C,0x0A,0x0F,0x09,0x0B,0x0D,0x0E};
const void display_controls() {
    std::cout << "Game Controls: " << std::endl;
    std::cout << (move_left[0]==' '?"space":std::string(1, move_left[0])) << " - move left" << std::endl;
    std::cout << (move_right[0]==' '?"space":std::string(1, move_right[0])) << " - move right" << std::endl;
    std::cout << (rotate_cw[0]==' '?"space":std::string(1, rotate_cw[0])) << " or " << (rotate_cw[2]==' '?"space":std::string(1, rotate_cw[2])) << " - rotate clockwise" << std::endl;
    std::cout << (rotate_ccw[0]==' '?"space":std::string(1, rotate_ccw[0])) << " - rotate counterclockwise" << std::endl;
    std::cout << (soft_drop[0]==' '?"space":std::string(1, soft_drop[0])) << " - soft drop" << std::endl;
    std::cout << (hard_drop[0]==' '?"space":std::string(1, hard_drop[0])) << " or " << (hard_drop[2]==' '?"space":std::string(1, hard_drop[2])) << " - hard drop" << std::endl;
    std::cout << "p - pause/resume" << std::endl;
}

// Globals
char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
WORD screen_colors[SCREEN_HEIGHT][SCREEN_WIDTH];
short next_block_in_box[4][2];
short block_coords[4][2];
short block_ID;
bool running=false;
bool no_block=true;
bool input_loop_running=false;
char last_key_from_input_loop;


// function for clearing the screen, USE THIS INSTEAD OF SYSTEM CLEAR
void cls( HANDLE hConsole )
{
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    if( !GetConsoleScreenBufferInfo( hConsole, &csbi )) return;
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    if( !FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten )) return;
    if( !GetConsoleScreenBufferInfo( hConsole, &csbi )) return;
    if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten )) return;
    SetConsoleCursorPosition( hConsole, coordScreen );
}
HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbi;

int random_number(int minimum, int maximum) {
    return rand()%(maximum-minimum+1)+minimum;
}
// converts a string to a 16 bit (signed) number if the parser does not come across an error
int StringToNumber(std::string input, short& informationpointer, short first=MIN_LEVEL, short last=MAX_LEVEL)
{
    informationpointer=1;
    int conversion[6]={0};
    if (last<first) {informationpointer=-1;return 0;}
    if (input.length()==0) {informationpointer=0;return 0;}
    bool negnum=false;
    if (input.at(0)==45)
    {
        if (first>=0) {informationpointer=0;return 0;}
        input.erase(0,1);
        negnum=true;
    }
    if (input.length()==0) {informationpointer=0;return 0;}
    while (input.length()>1) if (input.at(0)==48) input.erase(0,1); else break;
    for (short y=0; y<=input.length()-1; y++)
    {
        if (not(input.at(y)>=48 && input.at(y)<=57)) {informationpointer=0;return 0;}
        else conversion[input.length()-y]=(input.at(y)-48);
    }
    short xten=1;
    for (short y=1; y<=input.length(); y++)
    {
        short currentvalue=conversion[0];
        int tobeadded=conversion[y]*xten;
        conversion[0]+=tobeadded;
        if (conversion[0]<currentvalue || conversion[0]<tobeadded) {informationpointer=0;return 0;}
        xten*=10;
    }
    if (negnum) conversion[0] = -conversion[0];
    if (conversion[0] < first || conversion[0] > last) {informationpointer=0;return 0;}
    return conversion[0];
}
// unsigned integers up to 32 bits as the input
std::string ui_numtostr(size_t number)
{
    char c_number[10]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
    for (short i=0;i<=9;i++) {
        c_number[i]=(number%10)+48;
        number/=10;
        if (number==0) break;
    }
    std::string retstr="          ";
    short spos=0;
    for (short i=9;i>=0;i--) {
        if (c_number[i]==' ') continue;
        retstr.at(spos)=c_number[i];
        spos++;
    }
    if (retstr=="          ") {retstr="ERROR";return retstr;}
    for (short i=0;i<=9;i++) {
        if (retstr.at(i)==' ') {
            retstr.erase(i,retstr.length()-i);
            break;
        }
    }
    return retstr;
}
// (global) symbolic constants and global constants are used in reset_array
void reset_array()
{
    const short BEFORE_RIGHTBORDER_OFSCREEN=SCREEN_WIDTH-2;
    for (short row=0;row<=SCREEN_HEIGHT-1;row++) {
        screen[row][0]=BORDER_CHAR; // border
        for (short column=GAME_COORD_X;column<=border_rightof_game-1;column++) screen[row][column]=BACKGROUND_CHAR;
        screen[row][border_rightof_game]=BORDER_CHAR;
        screen[row][SCREEN_WIDTH-1]=BORDER_CHAR; // border
    }
    screen[Y_POSITION_OFNEXTBLOCK_BOX][X_POSITION_OFNEXTBLOCK_BOX]=201; // top left corner of the next block box
    screen[Y_POSITION_OFNEXTBLOCK_BOX][BEFORE_RIGHTBORDER_OFSCREEN]=187; // top right corner of the next block box
    if (SCREEN_WIDTH-GAME_WIDTH>=6) for (short row=Y_POSITION_OFNEXTBLOCK_BOX+1;row<=Y_POSITION_OFNEXTBLOCK_BOX+Y_LAST_ELEMENT_OFNEXTBLOCK_BOX-1;row++) {
        screen[row][X_POSITION_OFNEXTBLOCK_BOX]=186;
        for (short column=X_POSITION_OFNEXTBLOCK_BOX+1;column<=BEFORE_RIGHTBORDER_OFSCREEN-1;column++)
            screen[row][column]=BACKGROUND_CHAR;
        screen[row][BEFORE_RIGHTBORDER_OFSCREEN]=186;
    }
    screen[Y_POSITION_OFNEXTBLOCK_BOX+Y_LAST_ELEMENT_OFNEXTBLOCK_BOX][X_POSITION_OFNEXTBLOCK_BOX]=200; // bottom left corner of the next block box
    screen[Y_POSITION_OFNEXTBLOCK_BOX+Y_LAST_ELEMENT_OFNEXTBLOCK_BOX][BEFORE_RIGHTBORDER_OFSCREEN]=188; // bottom right corner of the next block box
    for (short column=X_POSITION_OFNEXTBLOCK_BOX+1;column<=BEFORE_RIGHTBORDER_OFSCREEN-1;column++) {
        screen[Y_POSITION_OFNEXTBLOCK_BOX][column]=205;
        screen[Y_POSITION_OFNEXTBLOCK_BOX+Y_LAST_ELEMENT_OFNEXTBLOCK_BOX][column]=205;
    }
    if (CLEAR_BELOW_NEXTBLOCK_BOX)
        for (short row=Y_POSITION_OFNEXTBLOCK_BOX+NEXTBLOCK_BOX_HEIGHT;row<=SCREEN_HEIGHT-1;row++) // below the box
            for (short column=displacement_side;column<=BEFORE_RIGHTBORDER_OFSCREEN;column++)
                screen[row][column]=BACKGROUND_CHAR;
    else {
        for (short row=Y_POSITION_OFNEXTBLOCK_BOX-NEXTBLOCK_BOX_HEIGHT;row>=0;row--) // above the box
            for (short column=displacement_side;column<=BEFORE_RIGHTBORDER_OFSCREEN;column++)
                screen[row][column]=BACKGROUND_CHAR;
    }
    if (SCREEN_WIDTH-GAME_WIDTH>=NEEDED_SPACE_FORSTRINGS_ANDVALUES) {
        screen[Y_POSITION_OFLEVEL][X_POSITION_OFSTRINGS]='L';
        screen[Y_POSITION_OFLEVEL][X_POSITION_OFSTRINGS+1]='e';
        screen[Y_POSITION_OFLEVEL][X_POSITION_OFSTRINGS+2]='v';
        screen[Y_POSITION_OFLEVEL][X_POSITION_OFSTRINGS+3]='e';
        screen[Y_POSITION_OFLEVEL][X_POSITION_OFSTRINGS+4]='l';
        screen[Y_POSITION_OFLEVEL][X_POSITION_OFSTRINGS+5]=':';
        screen[Y_POSITION_OFSCORE][X_POSITION_OFSTRINGS]='S';
        screen[Y_POSITION_OFSCORE][X_POSITION_OFSTRINGS+1]='c';
        screen[Y_POSITION_OFSCORE][X_POSITION_OFSTRINGS+2]='o';
        screen[Y_POSITION_OFSCORE][X_POSITION_OFSTRINGS+3]='r';
        screen[Y_POSITION_OFSCORE][X_POSITION_OFSTRINGS+4]='e';
        screen[Y_POSITION_OFSCORE][X_POSITION_OFSTRINGS+5]=':';
        screen[Y_POSITION_OFLINES][X_POSITION_OFSTRINGS]='L';
        screen[Y_POSITION_OFLINES][X_POSITION_OFSTRINGS+1]='i';
        screen[Y_POSITION_OFLINES][X_POSITION_OFSTRINGS+2]='n';
        screen[Y_POSITION_OFLINES][X_POSITION_OFSTRINGS+3]='e';
        screen[Y_POSITION_OFLINES][X_POSITION_OFSTRINGS+4]='s';
        screen[Y_POSITION_OFLINES][X_POSITION_OFSTRINGS+5]=':';
    }
    // color reset
    for (short row=0;row<=SCREEN_HEIGHT-1;row++) {
        for (short column=0;column<=SCREEN_WIDTH-1;column++)
            screen_colors[row][column]=DEFAULT_COLOR;
    }
}
// (global) symbolic constants and global constants are used in update_array
void update_array(const short level, const int score, const short lines)
{
    if (SCREEN_WIDTH-GAME_WIDTH>=NEEDED_SPACE_FORSTRINGS_ANDVALUES) {
        std::string levelstr = ui_numtostr(level);
        std::string scorestr = ui_numtostr(score);
        std::string linesstr = ui_numtostr(lines);
        short strpos=0;
        for (;strpos<=levelstr.length()-1;strpos++) screen[Y_POSITION_OFLEVEL_NUM][X_POSITION_OFVALUES+strpos]=levelstr.at(strpos);
        for (strpos=0;strpos<=scorestr.length()-1;strpos++) screen[Y_POSITION_OFSCORE_NUM][X_POSITION_OFVALUES+strpos]=scorestr.at(strpos);
        for (strpos=0;strpos<=linesstr.length()-1;strpos++) screen[Y_POSITION_OFLINES_NUM][X_POSITION_OFVALUES+strpos]=linesstr.at(strpos);
        if (SCREEN_WIDTH-GAME_WIDTH>=4)
            for (short row=Y_POSITION_OFNEXTBLOCK_BOX+1;row<=Y_POSITION_OFNEXTBLOCK_BOX+Y_LAST_ELEMENT_OFNEXTBLOCK_BOX-1;row++)
                for (short column=X_POSITION_OFNEXTBLOCK_BOX+1;column<=SCREEN_WIDTH-2-1;column++)
                    screen[row][column]=BACKGROUND_CHAR;
        for (short ipoints=0;ipoints<=3;ipoints++) {
            screen[block_coords[ipoints][0]][block_coords[ipoints][1]]=BLOCK_CHAR;
            screen[next_block_in_box[ipoints][0]][next_block_in_box[ipoints][1]]=BLOCK_CHAR;
        }
    }
}
void erase_tetromino() {
    COORD coord;
    for (short block=0;block<=3;block++) {
        if (block_coords[block][0]+start_of_screen>start_of_screen) {
            coord.X=block_coords[block][1]+INDENT_X;
            coord.Y=block_coords[block][0]+start_of_screen;
            SetConsoleCursorPosition(hStdout,coord);
            std::cout << BACKGROUND_CHAR;
        }
        screen_colors[block_coords[block][0]][block_coords[block][1]]=DEFAULT_COLOR;
    }
}
void print_tetromino() {
    COORD coord;
    SetConsoleTextAttribute( hStdout, tetromino_colors[block_ID] );
    for (short block=0;block<=3;block++) {
        if (block_coords[block][0]+start_of_screen>start_of_screen) {
            coord.X=block_coords[block][1]+INDENT_X;
            coord.Y=block_coords[block][0]+start_of_screen;
            SetConsoleCursorPosition(hStdout,coord);
            std::cout << screen[block_coords[block][0]][block_coords[block][1]];
            screen_colors[block_coords[block][0]][block_coords[block][1]]=tetromino_colors[block_ID];
        }
    }
    SetConsoleTextAttribute(hStdout, csbi.wAttributes);
}
void print_nexttetromino(short next_block_ID) {
    COORD newpos={INDENT_X+X_POSITION_OFNEXTBLOCK_BOX+1,start_of_screen+Y_POSITION_OFNEXTBLOCK_BOX};
    std::string spaces;
    for (short row=0;row<=Y_LAST_ELEMENT_OFNEXTBLOCK_BOX-2;row++) {
        newpos.Y++;
        SetConsoleCursorPosition(hStdout,newpos);
        spaces="";
        spaces.append(X_LAST_ELEMENT_OFNEXTBLOCK_BOX-2,BACKGROUND_CHAR);
        std::cout << spaces;
        for (short column=0;column<=X_LAST_ELEMENT_OFNEXTBLOCK_BOX-2;column++)
            screen_colors[newpos.Y][newpos.X+column]=DEFAULT_COLOR;
    }
    COORD coord;
    SetConsoleTextAttribute( hStdout, tetromino_colors[next_block_ID] );
    for (short block=0;block<=3;block++) {
        coord.X=next_block_in_box[block][1]+INDENT_X;
        coord.Y=next_block_in_box[block][0]+start_of_screen;
        SetConsoleCursorPosition(hStdout,coord);
        std::cout << screen[next_block_in_box[block][0]][next_block_in_box[block][1]];
        screen_colors[next_block_in_box[block][0]][next_block_in_box[block][1]]=tetromino_colors[next_block_ID];
    }
    SetConsoleTextAttribute(hStdout, csbi.wAttributes);
}
void print_values(const short& level, const int& score, const short& lines) {
    COORD newpos={INDENT_X+X_POSITION_OFVALUES,start_of_screen+Y_POSITION_OFLEVEL_NUM};
    std::string levelstr = ui_numtostr(level);
    SetConsoleCursorPosition(hStdout,newpos);
    std::cout << levelstr;
    std::string scorestr = ui_numtostr(score);
    newpos.Y=start_of_screen+Y_POSITION_OFSCORE_NUM;
    SetConsoleCursorPosition(hStdout,newpos);
    std::cout << scorestr;
    std::string linesstr = ui_numtostr(lines);
    newpos.Y=start_of_screen+Y_POSITION_OFLINES_NUM;
    SetConsoleCursorPosition(hStdout,newpos);
    std::cout << linesstr;
}
void flush_and_reset_cursor() {
    fflush(stdout);
    SetConsoleCursorPosition(hStdout,game_resting_position);
}
void display_screen()
{
    cls(hStdout);
    for (short indent_y=1;indent_y<=INDENT_Y;indent_y++) std::cout << std::endl;
    if (BORDERS) {
        for (short column=0;column<=SCREEN_WIDTH-1+INDENT_X;column++) std::cout << BORDER_CHAR;
        std::cout << std::endl;
    }
    for (short row=ABOVE_CEILING;row<=SCREEN_HEIGHT-1;row++) {
        if (FILL_LEFT) for (short indent_x=1;indent_x<=INDENT_X;indent_x++) std::cout << BORDER_CHAR;
        else for (short indent_x=1;indent_x<=INDENT_X;indent_x++) std::cout << BACKGROUND_CHAR;
        for (short column=0;column<=SCREEN_WIDTH-1;column++) {
            SetConsoleTextAttribute( hStdout, screen_colors[row][column] );
            std::cout << screen[row][column];
        }
        SetConsoleTextAttribute(hStdout, csbi.wAttributes);
        std::cout << std::endl;
    }
    if (BORDERS) {
        for (short column=0;column<=SCREEN_WIDTH-1+INDENT_X;column++) std::cout << BORDER_CHAR;
        std::cout << std::endl;
    }
}
bool block_below()
{
    bool lowest_block[4]={false,false,false,false};
    for (short X=GAME_COORD_X;X<=GAME_COORD_X+GAME_WIDTH-1;X++) {
        short previousposition;
        bool block_located=false;
        for (short check_block=0;check_block<=3;check_block++) {
            if (block_coords[check_block][1]==X) {
                if (block_located)
                    if (block_coords[check_block][0]<block_coords[previousposition][0])
                        continue;
                previousposition=check_block;
                block_located=true;
            }
        }
        if (block_located) lowest_block[previousposition]=true;
    }
    for (short check_below=0;check_below<=3;check_below++) {
        if (lowest_block[check_below]) {
            if (screen[block_coords[check_below][0]+1][block_coords[check_below][1]]==BLOCK_CHAR||block_coords[check_below][0]==(GAME_HEIGHT-1)||screen[block_coords[check_below][0]+1][block_coords[check_below][1]]==BORDER_CHAR)
                return true;
        }
    }
    return false;
}
short temp_block_coords[4][2];
bool rudimentary_rotate_counterclockwise() {
    for (short point=0;point<=3;point++)
        screen[block_coords[point][0]][block_coords[point][1]]=BACKGROUND_CHAR;
    temp_block_coords[0][0]=block_coords[0][0];
    temp_block_coords[0][1]=block_coords[0][1];
    for (short point=1;point<=3;point++) {
        temp_block_coords[point][0]=block_coords[0][0];
        temp_block_coords[point][1]=block_coords[0][1];
        if (!(block_coords[0][0]==block_coords[point][0]))
            temp_block_coords[point][1]=block_coords[0][1]-(block_coords[0][0]-block_coords[point][0]);
        if (!(block_coords[0][1]==block_coords[point][1]))
            temp_block_coords[point][0]=block_coords[0][0]+(block_coords[0][1]-block_coords[point][1]);
        if (screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BLOCK_CHAR||
        temp_block_coords[point][0]==GAME_HEIGHT||
        temp_block_coords[point][1]==(GAME_COORD_X-1)||
        temp_block_coords[point][1]==(GAME_COORD_X+GAME_WIDTH)||
        screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BORDER_CHAR)
            return false;
    }
    return true;
}
bool rudimentary_rotate_clockwise() {
    for (short point=0;point<=3;point++)
        screen[block_coords[point][0]][block_coords[point][1]]=BACKGROUND_CHAR;
    temp_block_coords[0][0]=block_coords[0][0];
    temp_block_coords[0][1]=block_coords[0][1];
    for (short point=1;point<=3;point++) {
        temp_block_coords[point][0]=block_coords[0][0];
        temp_block_coords[point][1]=block_coords[0][1];
        if (!(block_coords[0][0]==block_coords[point][0]))
            temp_block_coords[point][1]=block_coords[0][1]+(block_coords[0][0]-block_coords[point][0]);
        if (!(block_coords[0][1]==block_coords[point][1]))
            temp_block_coords[point][0]=block_coords[0][0]-(block_coords[0][1]-block_coords[point][1]);
        if (screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BLOCK_CHAR||
        temp_block_coords[point][0]==GAME_HEIGHT||
        temp_block_coords[point][1]==(GAME_COORD_X-1)||
        temp_block_coords[point][1]==(GAME_COORD_X+GAME_WIDTH)||
        screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BORDER_CHAR)
            return false;
    }
    return true;
}
void input_loop(const short &level, int &score, const short &lines, const short &next_block_ID)
{
    // Do not change block_coords or block_ID
    input_loop_running=true;
    const short points_when_dropped[2]=POINTS_AWARDED_WHEN_DROPPED;
    const short block_types[7][4][2]={ZIGGY_LEFT,ZIGGY_RIGHT,L_LEFT,L_RIGHT,LONG_FOUR,THREEWAY_INTERSECTION,SQUARE};
    while (running) {
        for (short point=0;point<=3;point++)
            screen[block_coords[point][0]][block_coords[point][1]]=BLOCK_CHAR;
        only_mutex.unlock();
        char key=getch();
        last_key_from_input_loop=key;
        if (!running) break;
        only_mutex.lock();
        if (key==move_left[0]||key==move_left[1]) {
            // move left
            bool continue_flag=false;
            for (short check_X=0;check_X<=3;check_X++)
                if (block_coords[check_X][1]==GAME_COORD_X) {continue_flag=true;break;}
            if (continue_flag) continue;
            bool leftmost_block[4]={false,false,false,false};
            for (short Y=GAME_COORD_Y;Y<=GAME_COORD_Y+GAME_HEIGHT-1;Y++) {
                short previousposition;
                bool block_located=false;
                for (short check_block=0;check_block<=3;check_block++) {
                    if (block_coords[check_block][0]==Y) {
                        if (block_located)
                            if (block_coords[check_block][1]>block_coords[previousposition][1])
                                continue;
                        previousposition=check_block;
                        block_located=true;
                    }
                }
                if (block_located) leftmost_block[previousposition]=true;
            }
            for (short check_collision=0;check_collision<=3;check_collision++) {
                if (leftmost_block[check_collision]) {
                    if (screen[block_coords[check_collision][0]][block_coords[check_collision][1]-1]==BLOCK_CHAR||block_coords[check_collision][1]==GAME_COORD_X||screen[block_coords[check_collision][0]][block_coords[check_collision][1]-1]==BORDER_CHAR) {
                        continue_flag=true;
                        break;
                    }
                }
            }
            if (continue_flag) continue;
            for (short clear_previous=0;clear_previous<=3;clear_previous++)
                screen[block_coords[clear_previous][0]][block_coords[clear_previous][1]]=BACKGROUND_CHAR;
            erase_tetromino();
            for (short move_left=0;move_left<=3;move_left++) block_coords[move_left][1]--;
            update_array(level,score,lines);
            print_tetromino();
            flush_and_reset_cursor();
        } else if (key==move_right[0]||key==move_right[1]) {
            // move right
            bool continue_flag=false;
            for (short check_X=0;check_X<=3;check_X++)
                if (block_coords[check_X][1]==GAME_COORD_X+GAME_WIDTH-1) {continue_flag=true;break;}
            if (continue_flag) continue;
            bool rightmost_block[4]={false,false,false,false};
            for (short Y=GAME_COORD_Y;Y<=GAME_COORD_Y+GAME_HEIGHT-1;Y++) {
                short previousposition;
                bool block_located=false;
                for (short check_block=0;check_block<=3;check_block++) {
                    if (block_coords[check_block][0]==Y) {
                        if (block_located)
                            if (block_coords[check_block][1]<block_coords[previousposition][1])
                                continue;
                        previousposition=check_block;
                        block_located=true;
                    }
                }
                if (block_located) rightmost_block[previousposition]=true;
            }
            for (short check_collision=0;check_collision<=3;check_collision++) {
                if (rightmost_block[check_collision]) {
                    if (screen[block_coords[check_collision][0]][block_coords[check_collision][1]+1]==BLOCK_CHAR||block_coords[check_collision][1]==(GAME_COORD_X+GAME_WIDTH-1)||screen[block_coords[check_collision][0]][block_coords[check_collision][1]+1]==BORDER_CHAR) {
                        continue_flag=true;
                        break;
                    }
                }
            }
            if (continue_flag) continue;
            for (short clear_previous=0;clear_previous<=3;clear_previous++)
                screen[block_coords[clear_previous][0]][block_coords[clear_previous][1]]=BACKGROUND_CHAR;
            erase_tetromino();
            for (short move_right=0;move_right<=3;move_right++) block_coords[move_right][1]++;
            update_array(level,score,lines);
            print_tetromino();
            flush_and_reset_cursor();
        } else if (key==rotate_cw[0]||key==rotate_cw[1]||key==rotate_cw[2]||key==rotate_cw[3]||key==rotate_ccw[0]||key==rotate_ccw[1]) {
            // w and x for rotating clockwise and z for rotating counterclockwise
            if (block_ID==2||block_ID==3||block_ID==5) {
                if (!((key==rotate_ccw[0]||key==rotate_ccw[1])?(rudimentary_rotate_counterclockwise()):(rudimentary_rotate_clockwise()))) continue;
            } else if (block_ID==0||block_ID==1) {
                if (!rudimentary_rotate_clockwise()) continue;
                if ((temp_block_coords[2][0]-temp_block_coords[3][0])==(block_types[block_ID][0][0]-block_types[block_ID][1][0])) {
                    bool goto_top=false;
                    if (block_ID==0) {
                        for (short point=0;point<=3;point++) {
                            temp_block_coords[point][0]--;
                            if (screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BLOCK_CHAR||
                            temp_block_coords[point][0]==GAME_HEIGHT||
                            screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BORDER_CHAR) {
                                goto_top=true;
                                break;
                            }
                        }
                    } else {
                        for (short point=0;point<=3;point++) {
                            temp_block_coords[point][0]++;
                            if (screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BLOCK_CHAR||
                            temp_block_coords[point][0]==GAME_HEIGHT||
                            screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BORDER_CHAR) {
                                goto_top=true;
                                break;
                            }
                        }
                    }
                    if (goto_top) continue;
                    short temp_coord[2]={temp_block_coords[0][0],temp_block_coords[0][1]};
                    temp_block_coords[0][0]=temp_block_coords[2][0];temp_block_coords[0][1]=temp_block_coords[2][1];
                    temp_block_coords[2][0]=temp_coord[0];temp_block_coords[2][1]=temp_coord[1];
                    temp_coord[0]=temp_block_coords[1][0];
                    temp_coord[1]=temp_block_coords[1][1];
                    temp_block_coords[1][0]=temp_block_coords[3][0];temp_block_coords[1][1]=temp_block_coords[3][1];
                    temp_block_coords[3][0]=temp_coord[0];temp_block_coords[3][1]=temp_coord[1];
                }
            } else if (block_ID==4) {
                for (short point=0;point<=3;point++)
                    screen[block_coords[point][0]][block_coords[point][1]]=BACKGROUND_CHAR;
                temp_block_coords[0][0]=block_coords[0][0];
                temp_block_coords[0][1]=block_coords[0][1];
                for (short point=1;point<=3;point++) {
                    temp_block_coords[point][0]=block_coords[0][0];
                    temp_block_coords[point][1]=block_coords[0][1];
                    if (!(block_coords[0][0]==block_coords[point][0]))
                        temp_block_coords[point][1]=block_coords[0][1]-(block_coords[0][0]-block_coords[point][0]);
                    if (!(block_coords[0][1]==block_coords[point][1]))
                        temp_block_coords[point][0]=block_coords[0][0]+(block_coords[0][1]-block_coords[point][1]);
                }
                bool goto_top=false;
                if ((temp_block_coords[2][0]-temp_block_coords[3][0])==(block_types[block_ID][0][0]-block_types[block_ID][1][0])) {
                    for (short point=0;point<=3;point++) {
                        temp_block_coords[point][1]++;
                        temp_block_coords[point][0]--;
                        if (screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BLOCK_CHAR||
                        temp_block_coords[point][0]==GAME_HEIGHT||
                        temp_block_coords[point][1]==(GAME_COORD_X-1)||
                        temp_block_coords[point][1]==(GAME_COORD_X+GAME_WIDTH)||
                        screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BORDER_CHAR) {
                            goto_top=true;
                            break;
                        }
                    }
                    if (goto_top) continue;
                    short temp_coord[2]={temp_block_coords[0][0],temp_block_coords[0][1]};
                    temp_block_coords[0][0]=temp_block_coords[2][0];temp_block_coords[0][1]=temp_block_coords[2][1];
                    temp_block_coords[2][0]=temp_coord[0];temp_block_coords[2][1]=temp_coord[1];
                    temp_coord[0]=temp_block_coords[1][0];
                    temp_coord[1]=temp_block_coords[1][1];
                    temp_block_coords[1][0]=temp_block_coords[3][0];temp_block_coords[1][1]=temp_block_coords[3][1];
                    temp_block_coords[3][0]=temp_coord[0];temp_block_coords[3][1]=temp_coord[1];
                } else {
                    for (short point=0;point<=3;point++) {
                        temp_block_coords[point][0]++;
                        if (screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BLOCK_CHAR||
                        temp_block_coords[point][0]==GAME_HEIGHT||
                        temp_block_coords[point][1]==(GAME_COORD_X-1)||
                        temp_block_coords[point][1]==(GAME_COORD_X+GAME_WIDTH)||
                        screen[temp_block_coords[point][0]][temp_block_coords[point][1]]==BORDER_CHAR) {
                            goto_top=true;
                            break;
                        }
                    }
                    if (goto_top) continue;
                }
            } else continue;
            erase_tetromino();
            for (short point=0;point<=3;point++) {
                block_coords[point][0]=temp_block_coords[point][0];
                block_coords[point][1]=temp_block_coords[point][1];
            }
            update_array(level,score,lines);
            print_tetromino();
            flush_and_reset_cursor();
        } else if (key==soft_drop[0]||key==soft_drop[1]) {
            // soft drop
            if (!block_below()) {
                erase_tetromino();
                for (short point=0;point<=3;point++) {
                    screen[block_coords[point][0]][block_coords[point][1]]=BACKGROUND_CHAR;
                    block_coords[point][0]++;
                }
                score+=points_when_dropped[0]*level;
                update_array(level,score,lines);
                print_tetromino();
                print_values(level,score,lines);
                flush_and_reset_cursor();
                if (block_below()) no_block=true;
            } else no_block=true;
        } else if (key==hard_drop[0]||key==hard_drop[1]||key==hard_drop[2]) {
            // hard drop
            no_block=true;
            erase_tetromino();
            while (!block_below()) {
                for (short point=0;point<=3;point++) block_coords[point][0]++;
                for (short point=0;point<=3;point++) screen[block_coords[point][0]-1][block_coords[point][1]]=BACKGROUND_CHAR;
                score+=points_when_dropped[1]*level;
            }
            update_array(level,score,lines);
            print_tetromino();
            print_values(level,score,lines);
            flush_and_reset_cursor();
        } else if (key=='p'||key=='P') {
            // pause the game
            char key_while_paused;
            screen[Y_PAUSE][X_POSITION_OFSTRINGS]='P';
            screen[Y_PAUSE][X_POSITION_OFSTRINGS+1]='a';
            screen[Y_PAUSE][X_POSITION_OFSTRINGS+2]='u';
            screen[Y_PAUSE][X_POSITION_OFSTRINGS+3]='s';
            screen[Y_PAUSE][X_POSITION_OFSTRINGS+4]='e';
            screen[Y_PAUSE][X_POSITION_OFSTRINGS+5]='d';
            display_screen();
            std::cout << "c - controls  q - quit";
            while (true) {
                key_while_paused=getch();
                if (key_while_paused=='p'||key_while_paused=='P') break;
                if (key_while_paused=='q'||key_while_paused=='Q') {
                    cls(hStdout);
                    std::cout << "Are you sure you want to quit?  (y/n)";
                    while (true) {
                        key_while_paused=getch();
                        if (key_while_paused=='y'||key_while_paused=='Y') {
                            std::cout << std::endl << "Press any key to quit...";
                            getch();
                            exit(0);
                        }
                        if (key_while_paused=='n'||key_while_paused=='N') break;
                    }
                    display_screen();
                    std::cout << "c - controls  q - quit";
                }
                if (key_while_paused=='c'||key_while_paused=='C') {
                    cls(hStdout);
                    display_controls();
                    std::cout << std::endl << "Press c to go back.";
                    bool resume=false;
                    while (true) {
                        key_while_paused=getch();
                        if (key_while_paused=='p'||key_while_paused=='P') {resume=true;break;}
                        if (key_while_paused=='c'||key_while_paused=='C') {
                            display_screen();
                            std::cout << "c - controls  q - quit";
                            break;
                        }
                    }
                    if (resume) break;
                }
            }
            for (short clear_paused=X_POSITION_OFSTRINGS;clear_paused<=X_POSITION_OFSTRINGS+5;clear_paused++)
                screen[Y_PAUSE][clear_paused]=BACKGROUND_CHAR;
            display_screen();
            continue;
        }
        //bool finish_wait_flag=false;
        // main_mutex.unlock()
        /*for (short ms=1;ms<=GAME_INPUT_DELAY;ms++) {
            Sleep(1);
            if (key==harddrop[0]||key==harddrop[1]||key==harddrop[2]) {
            // hard drop
            // delay after, then continue
            } else continue;
            continue_flag=true;
            break;
        }*/
        //Sleep(GAME_INPUT_DELAY); // count it and check last 3.. unlock it when it's sleeping
        // main_mutex.lock()
        // think of implementing a quit later, think of using ctrl
    }
    input_loop_running=false;
}
std::string getnumberfromuser()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}
void game_loop() {
    short start_level;
    do
    {
        cls(hStdout);
        std::cout << "Choose a level between " << FIRST_LEVEL << " and " << LAST_LEVEL << " to start at: ";
        std::string input=getnumberfromuser();
        short information;
        const short number=StringToNumber(input,information,FIRST_LEVEL,LAST_LEVEL);
         if (information==1) {
            start_level=number;
        } else if (information==0) {
            std::cout << "User Error: Invalid Entry" << std::endl;
            Sleep(1000);
            cls(hStdout);
            continue;
        } else if (information==-1) {
            std::cout << "Programmer Error: Invalid Use of StringToNumber()" << std::endl;
            Sleep(5000);
            cls(hStdout);
            continue;
        } else {
            std::cout << "Programmer Error: StringToNumber() Returned An Invalid Value" << std::endl;
            std::cout << "The program shall terminate (press any key to end the program...)" << std::endl;
            getch();
            exit(0);
        }
        break;
    } while(true);
    srand(time(NULL));
    no_block=true;
    int score=0;
    bool won=false;
    bool on_first_level=true;
    short level=start_level;
    short lines=0;
    short line_count_on_level=0;
    reset_array();
    display_screen();
    short blockfall_speed;
    if (level==1) blockfall_speed=STARTING_SPEED;
    else blockfall_speed=formula_const_a*pow(FORMULA_BASE_CONST,(difficulty_increase*level))+last_level_speed_minus_one;
    short next_block[4][2];
    short next_block_ID;
    const short block_types[7][4][2]={ZIGGY_LEFT,ZIGGY_RIGHT,L_LEFT,L_RIGHT,LONG_FOUR,THREEWAY_INTERSECTION,SQUARE};
    running=true;
    next_block_ID=random_number(0,6);
    for (short point=0;point<=3;point++) {
        next_block[point][0]=block_types[next_block_ID][point][0];
        next_block[point][1]=block_types[next_block_ID][point][1];
        next_block_in_box[point][0]=next_block[point][0];
        next_block_in_box[point][1]=next_block[point][1];
        next_block_in_box[point][0]+=Y_POSITION_OFNEXTBLOCK_BOX+2-Y_BLOCK_START;
        next_block_in_box[point][1]+=X_POSITION_OFNEXTBLOCK_BOX+4-X_BLOCK_START;
    }
    print_nexttetromino(next_block_ID);
    print_values(level,score,lines);
    // 2nd thread created below
    std::thread input_loop_thread([&level,&score,&lines,&next_block_ID]{return input_loop(level,score,lines,next_block_ID);});
    input_loop_thread.detach();
    // runs alongside main
    // when main (here) is in a rest loop (2 found below)
    // input_loop_thread will start processing input
    // when main (here) is not in a rest loop it is doing calculations and checking conditions
    // which takes very little time
    // input_loop_thread will lock when a key is pressed and will execute instructions based on which key is pressed
    // it will unlock when waiting for a key
    /// at the top of input_loop it sets the part of the screen where the tetromino's points/blocks are equal to BLOCK_CHAR
    do
    {
        only_mutex.try_lock();
        if (no_block) {
                /// WARNING: DO NOT UPDATE THE ARRAY BECAUSE IT WILL REPRINT THE BLOCKS!
            for (short point=0;point<=3;point++)
                screen_colors[block_coords[point][0]][block_coords[point][1]]=tetromino_colors[block_ID];
            short lines_justcleared=0;
            short lowest_Y=0;  // the highest Y, but it appears as the lowest
            for (short point=0;point<=3;point++)
                if (lowest_Y<block_coords[point][0]) lowest_Y=block_coords[point][0];
            bool full_line=true;
            for (short line_check=lowest_Y;line_check>=lowest_Y-3;line_check--) {
                if (line_check==0) break;
                full_line=true;
                for (short column=GAME_COORD_X;column<=GAME_COORD_X+GAME_WIDTH-1;column++) {
                    if (screen[line_check][column]==BACKGROUND_CHAR) {
                        full_line=false;
                        break;
                    }
                }
                if (full_line) {
                    for (short column=GAME_COORD_X;column<=GAME_COORD_X+GAME_WIDTH-1;column++)
                        screen[line_check][column]=BACKGROUND_CHAR;
                    for (short row=line_check;row>=1;row--)
                        for (short column=GAME_COORD_X;column<=GAME_COORD_X+GAME_WIDTH-1;column++) {
                            screen[row][column]=screen[row-1][column];
                            screen_colors[row][column]=screen_colors[row-1][column];
                        }
                    line_check++;
                    lowest_Y--;
                    lines_justcleared++;
                    Sleep(100);
                    display_screen();
                    lines++;
                    print_values(level,score,lines);
                    flush_and_reset_cursor();
                }
            }
            if (lines_justcleared>0) {
                line_count_on_level+=lines_justcleared;
                const short points_awarded_when_cleared[4]=POINTS_AWARDED_WHEN_CLEARED;
                score+=points_awarded_when_cleared[lines_justcleared-1]*level;
                if (on_first_level) {
                    if (line_count_on_level>=((start_level-1)*10+10)||line_count_on_level>=(((100<((start_level-1)*10-50))?((start_level-1)*10-50):100))) {
                        line_count_on_level=0;
                        level++;
                        blockfall_speed=formula_const_a*pow(FORMULA_BASE_CONST,(difficulty_increase*level))+last_level_speed_minus_one;
                        if (level>LAST_LEVEL) {
                            won=true;
                            running=false;
                        }
                        on_first_level=false;
                    }
                } else {
                    if (line_count_on_level>=10) {
                        line_count_on_level=0;
                        level++;
                        if (level==LAST_LEVEL) blockfall_speed=LAST_LEVEL_SPEED;
                        else if (level>LAST_LEVEL) {
                            won=true;
                            running=false;
                        } else blockfall_speed=formula_const_a*pow(FORMULA_BASE_CONST,(difficulty_increase*level))+last_level_speed_minus_one;
                    }
                }
                display_screen();
                print_values(level,score,lines);
                flush_and_reset_cursor();
            }
            block_ID=next_block_ID;
            next_block_ID=random_number(0,6);
            for (short point=0;point<=3;point++) {
                block_coords[point][0]=next_block[point][0];
                block_coords[point][1]=next_block[point][1];
                if (screen[block_coords[point][0]][block_coords[point][1]]==BLOCK_CHAR) {
                    running=false;
                    break;
                }
                next_block[point][0]=block_types[next_block_ID][point][0];
                next_block[point][1]=block_types[next_block_ID][point][1];
                next_block_in_box[point][0]=next_block[point][0];
                next_block_in_box[point][1]=next_block[point][1];
                next_block_in_box[point][0]+=Y_POSITION_OFNEXTBLOCK_BOX+2-Y_BLOCK_START;
                next_block_in_box[point][1]+=X_POSITION_OFNEXTBLOCK_BOX+4-X_BLOCK_START;
            }
            if (!running) {only_mutex.unlock();break;}
            no_block=false;
            update_array(level,score,lines);
            print_tetromino();
            print_nexttetromino(next_block_ID);
            flush_and_reset_cursor();
        }
        only_mutex.unlock();
        if (!no_block) { Sleep(blockfall_speed%10);
        for (short fall=1;fall<=blockfall_speed/10;fall++) {
            if (!no_block) Sleep(10);
            else break;
        }}
        only_mutex.lock();
        if (block_below()) {
            if (no_block) continue;
            only_mutex.unlock();
            Sleep(blockfall_speed%10);
            for (short fall=1;fall<=blockfall_speed/10;fall++) Sleep(10);
            only_mutex.lock();
            if (block_below()) {no_block=true;continue;}
        }
        erase_tetromino();
        for (short point=0;point<=3;point++) block_coords[point][0]++;
        for (short point=0;point<=3;point++) screen[block_coords[point][0]-1][block_coords[point][1]]=BACKGROUND_CHAR;
        update_array(level,score,lines);
        print_tetromino();
        flush_and_reset_cursor();
    } while(true);
    std::cout << "You ";
    if (won) std::cout << "won";
    else std::cout << "lost";
    std::cout << ".  Play again?  (y/n)";
    char key;
    if (!input_loop_running) {key=getch();last_key_from_input_loop=key;}
    while (input_loop_running) Sleep(0);
    key=last_key_from_input_loop;
    do {
        if (key=='y'||key=='Y') {break;}
        else if (key=='n'||key=='N') {return;}
        else {key=getch();continue;}
    } while (true);
    game_loop();
}
int main()
{
    if (STARTING_SPEED<LAST_LEVEL_SPEED||FIRST_LEVEL>LAST_LEVEL||FIRST_LEVEL<MIN_LEVEL||LAST_LEVEL>MAX_LEVEL)
        return EXIT_FAILURE;
    system("TITLE Tetris");  // system command feels faster
    SetConsoleTextAttribute(hStdout,DEFAULT_COLOR);
    GetConsoleScreenBufferInfo( hStdout, &csbi );
    std::cout << "Welcome to Tetris:" << std::endl;
    std::cout << "(P)lay" << std::endl;
    std::cout << "(Q)uit" << std::endl << std::endl;
    display_controls();
    do
    {
        char key=getch();
        if (key=='p'||key=='P') break;
        else if (key=='q'||key=='Q') {
            cls(hStdout);
            std::cout << "Press any key to quit...";
            getch();
            return 0;
        } else continue;
    } while(true);
    game_loop();
    cls(hStdout);
    std::cout << "Press any key to quit...";
    getch();
    return 0;
}
