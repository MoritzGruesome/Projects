#include <stdint.h>
#include <pic32mx.h>
#include "gamefunc.h"
#include "utils.h"

void snakebodyINIT ( void ) { // Viktor
    // ititializes the snake body array to zero
    int i = 0;
    for(i = 0; i < 256; i++) {
        snakebody1[i].x = 0;
        snakebody1[i].y = 0;
        
    }
}

void BotINIT ( void ) { // Viktor
    // ititializes the snake body array to zero
    int i = 0;
    for(i = 0; i < 256; i++) {
        Botsnake[i].x = 0;
        Botsnake[i].y = 0;
        
    }
}

// prints square of side length "length", coordinates based on top left corner.
void printsquare(char cornerx, char cornery, char length) { // Viktor
    char x;
    char y;
    for(y = cornery; y <= length + cornery; y++) {
        for(x = cornerx; x <= length + cornerx; x++) {
            d_mat[y][x] = 1;
        }
    }
    
}

void printapple(char cornerx, char cornery) { // Viktor
    //prints the three pixle apple with the argument coordinates
    d_mat[cornery][cornerx] = 1;
    d_mat[cornery+1][cornerx] = 1;
    d_mat[cornery+1][cornerx+1] = 1;
}

void printsnake1(int length) { // Moritz
    // prints each body part consecutively
    int i = 0;
    
    for(i = 0; i < length; i++)
    {
        printsquare(snakebody1[i].x, snakebody1[i].y, 2);
    }
    //disp_Write();
    //time_Tick(0.0005);
    Timer3(16000);
    
}

void printBot(int length) { // Moritz
    // prints each body part consecutively, however for the Bot snake
    int i = 0;
    
    for(i = 0; i < length; i++)
    {
        printsquare(Botsnake[i].x, Botsnake[i].y, 2);
    }
    //disp_Write();
    
    
    
}
void cleardisp( void ) {  // Viktor
    // clears the entire displaybuffer, setting the screen black
    //however leaves the white 1 pixel border
    int i;
    int j;

    for(i = 1; i < 31; i++) {               // initializes the screen to fully on
        for(j = 1; j < 127; j++) {
            d_mat[i][j] = 0;
        }
    }
}

void lightdisplay( void ) { // Viktor
    //makes the entire screen white
    //this is done purely to create the white border
    int i;
    int j;

    for(i = 0; i < 32; i++) {               // initializes the screen to fully on
        for(j = 0; j < 128; j++) {
            d_mat[i][j] = 1;
        }
    }
}

void createHead1( void ) {  // Moritz
    // creates the head of the snake by initializing its coordinates
    snakebody1[0].x = 75;
    snakebody1[0].y = 25;
}

void createBotHead( void ) {    //Moritz
    // creates the head of the snake
    Botsnake[0].x = 100;
    Botsnake[0].y = 10;
}

void StraightRight1( void ) {   //Viktor
    // the following 4 functions change the head coordinates of the snake
    snakebody1[0].x += 1;
}

void StraightLeft( void ) { //Moritz
    snakebody1[0].x -= 1;
}

void StraightUp( void ) {       //Viktor
    snakebody1[0].y -= 1;
}

void StraightDown( void ) {     //Moritz
    snakebody1[0].y += 1;
}

void BotRight( void ) {     //Viktor
    // the following 4 functions change the head coordinates of the snake
    Botsnake[0].x += 1;
}

void BotLeft( void ) {      //Moritz
    Botsnake[0].x -= 1;
}

void BotUp( void ) {        //Viktor
    Botsnake[0].y -= 1;
}

void BotDown( void ) {      //Moritz
    Botsnake[0].y += 1;
}


    
void snakeHistory1( void ) {    //Moritz
    // snake data is shifted so that the most recent head position is the first item in the array
    int i;
    for(i = 255; i > 0; i--) {
        snakebody1[i] = snakebody1[i-1];
        }
}

void BotHistoryEasy( void ) {   //Moritz
    // snake data is shifted so that the most recent head position is the first item in the array
    
        int i;
            for(i = 255; i > 0; i--) {
                Botsnake[i] = Botsnake[i-1];
            }
    }



bool collision(int length ) {   //Moritz
    // checks collision of snake with itself, by checking overlap of bodyparts
    int i;
    for(i = 15; i < length; i++) {
        if ((snakebody1[0].x > snakebody1[i].x -2 ) && (snakebody1[0].x < snakebody1[i].x + 2))
        {
            if ((snakebody1[0].y > snakebody1[i].y-2) && (snakebody1[0].y < snakebody1[i].y + 2)) {
                return true;
            }
        }
    }
    return false;
}

bool BotCollision(int length ) {        //Moritz
    // checks collision of snake with bot snake
    int i;
    for(i = 0; i < length; i++) {
        if ((snakebody1[0].x > Botsnake[i].x -2 ) && (snakebody1[0].x < Botsnake[i].x + 2))
        {
            if ((snakebody1[0].y > Botsnake[i].y-2) && (snakebody1[0].y < Botsnake[i].y + 2)) {
                return true;
            }
        }
    }
    return false;
}



char randintx( int length ) { // Moritz
    // pseudorandom integer generator based off of tail position of snake
    // the coordinates are bounded by the & and | operators
    char value = (((snakebody1[length-1].x)/2 +snakebody1[length-1].y) & 0x7f | 0x05);
    return value;
}

char randinty( int length ) {  //Moritz
    // two generators for x,y since the range of integer is different for the two
    char value = (((snakebody1[length-1].x)/2 + snakebody1[length-1].y) & 0x19 | 0x05);
    return value;
}



char font_h[38][48] = { //Viktor
    // 2D array with encoded characters for letters
    // 'A'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,1,1,1,0,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,0,0,1,0,},
    // 'B'
    {0,0,1,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,1,1,1,1,0,0,},
    // 'C'
    {0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,1,0,0,0,0,0,0,
        0,1,0,0,0,0,0,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0,},
    // 'D'
    {0,1,1,1,1,0,0,0,
        0,1,0,0,0,1,0,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,0,1,0,0,
        0,1,1,1,1,0,0,0,},
    // 'E'
    {0,0,1,1,1,1,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,},
    // 'F'
    {0,0,1,1,1,1,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,},
    // 'G'
    {0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,1,0,0,0,0,0,0,
        0,1,0,0,1,1,1,0,
        0,1,0,0,0,0,1,0,
        0,0,1,1,1,1,1,0,},
    // 'H'
    {0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,1,1,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,},
    // 'I'
    {0,0,0,1,1,1,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,1,1,1,0,0,},
    // 'J'
    {0,0,0,0,1,1,1,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
    // 'K'
    {0,1,0,0,1,0,0,0,
        0,1,0,1,0,0,0,0,
        0,1,1,0,0,0,0,0,
        0,1,0,1,0,0,0,0,
        0,1,0,0,1,0,0,0,
        0,1,0,0,0,1,0,0,},
    // 'L'
    {0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,},
    // 'M'
    {0,1,0,0,0,0,1,0,
        0,1,1,0,0,1,1,0,
        0,1,0,1,1,0,1,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,0,0,1,0,},
    // 'N'
    {0,1,0,0,0,0,1,0,
        0,1,1,0,0,0,1,0,
        0,1,0,1,0,0,1,0,
        0,1,0,0,1,0,1,0,
        0,1,0,0,0,1,1,0,
        0,1,0,0,0,0,1,0,},
    // 'O'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,0,0,1,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
    // 'P'
    {0,0,1,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,},
    // 'Q'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,1,0,0,0,0,1,0,
        0,1,0,0,1,0,1,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,1,0,},
// 'R'
    {0,0,1,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,0,1,0,},
// 'S'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
// 'T'
    {0,0,1,1,1,1,1,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,},
// U
    {0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
// 'V'
    {0,1,0,0,0,0,1,0,
        0,1,0,0,0,0,1,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
// 'W'
    {1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        0,1,0,0,0,0,1,0,
        0,1,0,1,1,0,1,0,
        0,1,0,1,1,0,1,0,
        0,0,1,0,0,1,0,0,},
// 'X'
    {0,1,0,0,0,0,1,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,1,0,0,0,0,1,0,},
// 'Y'
    {0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,},
// 'Z'
    {0,0,1,1,1,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,},
// '0'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
// '1'
    {0,0,0,0,1,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,1,1,1,0,0,},
// '2'
    {0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,1,1,0,0,
        0,0,0,1,0,0,0,0,
        0,0,1,1,1,1,1,0,},
// '3'
    {0,0,1,1,1,1,1,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,1,0,0,
        0,0,0,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0,},
// '4'
    {0,0,0,0,0,1,0,0,
        0,0,0,0,1,1,0,0,
        0,0,0,1,0,1,0,0,
        0,0,1,1,1,1,1,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,1,0,0,},
// '5'
    {0,0,1,1,1,1,1,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,0,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,0,},
// '6'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
// '7'
    {0,0,1,1,1,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,0,1,0,0,0,0,},
// '8'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},
// '9'
    {0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,1,1,0,0,0,},

    {0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,},
    
    {1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,}
};


//following arrays are the coordinates of the obstacles for Hard and Medium difficulty setting for single player mode
char mediumObstacles[5][2] = {{70,1},{100,13},{1,27},{32,27},{32,23}};
char hardObstacles[7][2] = {{70,1},{100,13},{1,27},{32,27},{32,23},{15,10},{45,10}};

void writeletter(int x, int y, int index) { //Moritz
    //this writes the encoded character to the display buffer in the argumented coordinates
    int i;
    int j;
    int k = 0;
    for(i = y; i < y+6; i++) {
        for(j = x; j < x+8; j++) {
            d_mat[i][j] = font_h[index][k++];
        }
    }
}

void keyboard() {  // Moritz
    // sends a keyboard layout to the display buffer, used for typing 3 letter initial
    int i;
    for(i = 0; i < 13; i++) {
        writeletter(12+(8*i),19,i);
        writeletter(12+(8*i),26,i+13);
    }
}

int SCOREBOARD[10] = {18,2,14,17,4,1,14,0,17,3};    //Moritz
// character encoding for words: SCOREBOARD, P1, PVSAI
int P[2] = {27,15};
int PVSAI[8] = {28,15,36,21,18,36,0,8};

void menuScreen() {
    // character encoding of the main menu screen
    int i;
    int a;
    
    for(i = 0; i < 10; i++) {
        a = SCOREBOARD[i];
        writeletter(24+(8*i),8,a);
    }
    for(i = 0; i < 2; i++) {
        a = P[i];
        writeletter(58+(8*i),16,a);
    }
    for(i = 0; i < 8; i++) {
        a = PVSAI[i];
        writeletter(36+(8*i),24,a);
    }
}

void difficultyScreen() {  //Moritz
    //character encoding of the difficulty select for menu
    int i;
    int a;
    int EASY[4] = {4,0,18,24};
    int MEDIUM[6] = {12,4,3,8,20,12};
    int HARD[4] = {7,0,17,3};
    
    for(i = 0; i < 4; i++) {
        a = EASY[i];
        writeletter(48+(8*i),8,a);
    }
    for(i = 0; i < 6; i++) {
        a = MEDIUM[i];
        writeletter(40+(8*i),16,a);
    }
    for(i = 0; i < 4; i++) {
        a = HARD[i];
        writeletter(48+(8*i),24,a);
    }
}

void gameTypeScreen() {  //Viktor
    // character encoding for game type(1P or 2P)
    int i;
    int a;
    
    for(i = 0; i < 2; i++) {
        a = P[i];
        writeletter(58+(8*i),10,a);
    }
    for(i = 0; i < 8; i++) {
        a = PVSAI[i];
        writeletter(36+(8*i),20,a);
    }
}

char HundredthToChar( int num ) { //Moritz
    //extracts the most significant digit of score
    char hundredth = 0;
    while (num > 100) {
        num -= 100;
        hundredth += 1;
    }
    return hundredth;
}

char TenthToChar(int num) {  //Viktor
    //extracts the tenth digit from score
    char hundredth = 0;
        char tenth = 0;
        
        while (num > 100) {
            num -= 100;
            hundredth += 1;
        }
        while (num > 10) {
            num -= 10;
            tenth += 1;
            
        }
    return tenth;
}

char OnethToChar(int num) {  //Viktor
    // extracts the least significant digit from score
    char hundredth = 0;
        char tenth = 0;
        char oneth = 0;
        while (num > 100) {
            num -= 100;
            hundredth += 1;
        }
        while (num > 10) {
            num -= 10;
            tenth += 1;
            
        }
        while (num > 0) {
            num -= 1;
            oneth += 1;
        }
    return oneth;
}
// Moritz
void ScoreBoardScreen(struct player p1, struct player p2, struct player p3) {
    //encoding for displaybuffer showing the scoreboard for different game modes
    time(5000);
    time(5000);
    time(5000);
    while(btn(4) != 0x80) {
        cleardisp();
        writeletter(20,8,27);
        writeletter(44,8,p1.initials[0]);
        writeletter(52,8,p1.initials[1]);
        writeletter(60,8,p1.initials[2]);
        writeletter(84,8,p1.scoreletters[0]);
        writeletter(92,8,p1.scoreletters[1]);
        writeletter(100,8,p1.scoreletters[2]);
        
        writeletter(20,16,28);
        writeletter(44,16,p2.initials[0]);
        writeletter(52,16,p2.initials[1]);
        writeletter(60,16,p2.initials[2]);
        writeletter(84,16,p2.scoreletters[0]);
        writeletter(92,16,p2.scoreletters[1]);
        writeletter(100,16,p2.scoreletters[2]);
        
        writeletter(20,24,29);
        writeletter(44,24,p3.initials[0]);
        writeletter(52,24,p3.initials[1]);
        writeletter(60,24,p3.initials[2]);
        writeletter(84,24,p3.scoreletters[0]);
        writeletter(92,24,p3.scoreletters[1]);
        writeletter(100,24,p3.scoreletters[2]);
        updateDisp();
    }
    cleardisp();
}

//8,16,26
char menuselect( void ) {  //Moritz
    //returns a value which is used to specifiy the directory of the main menu
    //we create a menu system guided by a black cursor which is used to navigate menu and to select if you want to play or view the scoreboard aswell as selecting difficulty levels
    cleardisp();
    char y = 8;
    int i;
    time(5000);
    time(5000);
    while(btn(4) != 0x80){
        menuScreen();
        for(i = 0; i < 10; i++) {
            writeletter(20+8*i, y, 36);
        }
        updateDisp();
        if((btn(1) == 0x02) && (y < 24)) {
            time(5000);
            time(5000);
            time(5000);
            y += 8;
        }
        if((btn(2) == 0x20) && (y > 8)) {
            time(5000);
            time(5000);
            time(5000);
            y -= 8;
        }
    }
    cleardisp();
    return y;
}

char difficultyselect( void ) {  //Viktor
    // returns a value based on the difficulty level of the game
    char y = 8;
    int i;
    time(5000);
    time(5000);
    while(btn(4) != 0x80){
        difficultyScreen();
        for(i = 0; i < 10; i++) {
            writeletter(20+8*i, y, 36);
        }
        updateDisp();
        if((btn(1) == 0x02) && (y < 24)) {
            time(5000);
            time(5000);
            time(5000);
            y += 8;
        }
        if((btn(2) == 0x20) && (y > 8)) {
            time(5000);
            time(5000);
            time(5000);
            y -= 8;
        }
    }
    cleardisp();
    return y;
}

char gametypeselect( void ) {  //Moritz
    // returns a value based on the game type(1P or 2P)
    time(5000);
    time(5000);
    char y = 10;
    int i;
    while(btn(4) != 0x80){
        gameTypeScreen();
        for(i = 0; i < 10; i++) {
            writeletter(20+8*i, y, 36);
        }
    
        updateDisp();
        if((btn(1) == 0x02) && (y < 20)) {
            time(5000);
            time(5000);
            time(5000);
            y += 10;
        }
        if((btn(2) == 0x20) && (y > 10)) {
            time(5000);
            time(5000);
            time(5000);
            y -= 10;
        }
    }
    cleardisp();
    return y;
}

void typename(char i, char j, char k ) {  //Moritz
    // function takes in 3 index values used to locate the correct scoreboard data, i selects the game type, j selects the difficulty, k selects the position on the scoreboard(1-3)
    // based on letters selected on the keyboard, the corresponding index of the character in the font array is appended to the 3 element initials array which is an attribute of the player struct
    int x = 12;
    int y = 19;
    int index = 0;
    char pindex = 0;
    scoretable[i][j][k].initials[0] = 36;
    scoretable[i][j][k].initials[1] = 36;
    scoretable[i][j][k].initials[2] = 36;
    
    bool type = true;
    while (type) {
        
        keyboard();
        writeletter(x,y,36);
        updateDisp();
        
        
        if (btn(1) == 0x2){
            time(5000);
            time(5000);
            time(5000);
            switch(y) {
                case(19):
                    switch(x) {
                        case(108):
                            x = 12;
                            y = 26;
                            index += 1;
                            break;
                        default:
                            x += 8;
                            index += 1;
                    }
                    break;
                case(26):
                    switch(x) {
                        case(108):
                            break;
                        default:
                            x += 8;
                            index += 1;
                    }
                    break;
            }
            }
        
        if (btn(2) == 0x20){
            time(5000);
            time(5000);
            time(5000);
            switch(y) {
                case(19):
                    switch(x) {
                        case(12):
                            break;
                        default:
                            x -= 8;
                            index -= 1;
                    }
                    break;
                case(26):
                    switch(x) {
                        case(12):
                            x = 116;
                            y = 19;
                            index -= 1;
                        default:
                            x -= 8;
                            index -= 1;
                    }
                    break;
            }
            }
        if (btn(4) == 0x80) {
            time(5000);
            time(5000);
            time(5000);
            scoretable[i][j][k].initials[pindex++] = index;
        }
        writeletter(52,10,scoretable[i][j][k].initials[0]);
        writeletter(60,10,scoretable[i][j][k].initials[1]);
        writeletter(68,10,scoretable[i][j][k].initials[2]);
        
        if (pindex == 4) {
            time(5000);
            time(5000);
            time(5000);
            time(5000);
            time(5000);
            time(5000);
            type = false;
        }
    }
    cleardisp();
}

void mediumMAP( void ) {   // Viktor
    // encodes the medium level map obstacles to the displaybuffer
    printsquare(70,1,4);
    printsquare(100,13,4);
    printsquare(1,27,4);
    printsquare(32,27,4);
    printsquare(32,23,4);
}

void hardMAP( void ) {   // Moritz
    //encodes the hard level map obstacles to the displaybuffer
    printsquare(70,1,4);
    printsquare(100,13,4);
    printsquare(1,27,4);
    printsquare(32,27,4);
    printsquare(32,23,4);
    printsquare(15,10,4);
    printsquare(45,10,4);
}

bool mediumCollide() {  //Viktor
    //returns boolean value based on whether player snake has collided with obstacles in the medium difficulty setting
    int i;
    for(i = 0;i < 5; i++) {
        if ((snakebody1[0].x > mediumObstacles[i][0] -4 ) && (snakebody1[0].x < mediumObstacles[i][0] + 4))
        {
            if ((snakebody1[0].y > mediumObstacles[i][1]-4) && (snakebody1[0].y < mediumObstacles[i][1] + 4)) {
                return true;
            }
        }
    }
    return false;
}

bool hardCollide() {  //Moritz
    //returns boolean value based on whether player snake has collided with obstacles in the hard difficulty setting
    int i;
    for(i = 0;i < 7; i++) {
        if ((snakebody1[0].x > hardObstacles[i][0] -4 ) && (snakebody1[0].x < hardObstacles[i][0] + 4))
        {
            if ((snakebody1[0].y > hardObstacles[i][1]-4) && (snakebody1[0].y < hardObstacles[i][1] + 4)) {
                return true;
            }
        }
    }
    return false;
}

char randomMediumx(int length, char y) {  //Moritz
    //special case for medium setting apple coordinates. when the apple is spawned within an obstacle, the apples x coordinate is shifted right thereby avoiding the obstacle
    char x = randintx(length);
    int i;
    for(i = 0;i < 5; i++) {
        if ((x >= mediumObstacles[i][0] -2 ) && (x <= mediumObstacles[i][0] + 4))
        {
            if ((y >= mediumObstacles[i][1]-2) && (y <= mediumObstacles[i][1] + 4)) {
                x += 5;
            }
        }
    }
    return x;
}

char randomHardx(int length, char y) {  // Viktor
    //special case for hard setting apple coordinates, same idea as the previous function, however with more obstacles to check
    char x = randintx(length);
    int i;
    for(i = 0;i < 7; i++) {
        if ((x > hardObstacles[i][0] -2 ) && (x < hardObstacles[i][0] + 4))
        {
            if ((y > hardObstacles[i][1]-2) && (y < hardObstacles[i][1] + 4)) {
                x += 5;
            }
        }
    }
    return x;
}

int snakeMEDIUM( void ) {   //Moritz
    //one of 4 game functions. the four game functions are largely the same, so some things will not be repeated following this description, however differences will be highlighted. The four game functions allows the program to easily initiate different games with different settings. Since the player score is the only thing we are interested in, we return the snakelength value at the end of the function. The higher the snakelength, the better the player has done.

snakebodyINIT();
spi_PowerOnDisplay();
createHead1();
// initializes the snake, by setting head coordinates, powering on display, clearing the snakebody data

int snakelength = 16;
//initializes the starting length of the snake to 16 pixels
bool playing = true;
//when playing is true the main game loop is iterated
bool wallcollision = false;
//condition set to true when head of snake touches the wall
bool ateapple = false;
//condition set to true when head of snake overlaps the apple
char direction = 0x01;
//holds direction of snake, so that it may move indefinately in that direction without input

char applex = 28;
char appley = 13;
//coordinates of the apple

lightdisplay();
    time(5000);
    time(5000);
//lights the screen, to create pixle wall
while(playing) {
//main game loop
    
    if ((snakebody1[0].x > applex -2 ) && (snakebody1[0].x < applex + 2))           // checks the overlap between head and apple
    {
        if ((snakebody1[0].y > appley-2) && (snakebody1[0].y < appley + 2)) {
            ateapple = true;
        }
    }
    
    if (ateapple) {
        //checks if the apple has been eaten or not
        appley = randinty(snakelength);
        applex = randomMediumx(snakelength, appley);
        snakelength += 8;
        //if the apple is eaten then the length is incremented by 8
        ateapple = false;
        //condition is reset
    }
    //following 4 if statements change the direction variable when buttons on pic32 are pressed
    if (btn(1) == 0x2)      // right
    {
        direction = 0x01;
    }
    
    if (btn(2) == 0x20)     // left
    {
        direction = 0x02;
    }
    
    if (btn(3) == 0x40)     // up
    {
        direction = 0x04;
    }
    
    if (btn(4) == 0x80)     // down
    {
        direction = 0x08;
    }
    //coordinates of snake head are changed based on the direction variable
    switch (direction) {
        case 0x01:
            StraightRight1();
            if (snakebody1[0].x >= 123) {
                //when the snake head exceeds the boundaries of the wall, wallcollision is set to true
                wallcollision = true;
            }
            
            break;
        case 0x02:
            StraightLeft();
            if (snakebody1[0].x <= 0) {
                wallcollision = true;
            }
            
            break;
        case 0x04:
            StraightUp();
            if (snakebody1[0].y <= 0) {
                wallcollision = true;
            }
            
            break;
        case 0x08:
            StraightDown();
            if (snakebody1[0].y >= 29) {
                wallcollision = true;
            }
            
            break;
    }
    
    snakeHistory1();
    //function shifts the elements in the array snakebody1 to the right, by doing this in every iteration we ensure that the most current movement of the head is stored in the least significant position of the array. each index to the right of this then represents a time step backwards of the snake
    playing = !collision(snakelength) && !wallcollision && !mediumCollide();  //stops game loop after collisions
    printsnake1(snakelength);
    //encodes the snake into the display buffer based on the coordinates stored in the struct object of snakebody1
    
    printapple(applex,appley);
    //prints apple on screen
    mediumMAP();
    //prints the obstacles for medium difficulty
    updateDisp();
    //screen reads data from the displaybuffer
    cleardisp();
    //clears the screen
}
cleardisp();
updateDisp();
return snakelength;
//returns the score of the game
}

int snakeHARD( void ) { // Viktor

snakebodyINIT();
spi_PowerOnDisplay();
createHead1();

int snakelength = 16;
bool playing = true;
bool wallcollision = false;
bool ateapple = false;
char direction = 0x01;

char applex = 28;
char appley = 16;

lightdisplay();
    time(5000);
    time(5000);
while(playing) {
    //main game loop
    
    if ((snakebody1[0].x > applex -2 ) && (snakebody1[0].x < applex + 2))           // checks the overlap between head and apple
    {
        if ((snakebody1[0].y > appley-2) && (snakebody1[0].y < appley + 2)) {
            ateapple = true;
        }
    }
    
    if (ateapple) {
        // checks if the apple has been eaten or not
        //applex = randintx(snakelength);
        appley = randinty(snakelength);
        applex = randomHardx(snakelength,appley);
        snakelength += 8;
        ateapple = false;
    }
    
    if (btn(1) == 0x2)      // right
    {
        direction = 0x01;
    }
    
    if (btn(2) == 0x20)     // left
    {
        direction = 0x02;
    }
    
    if (btn(3) == 0x40)     // up
    {
        direction = 0x04;
    }
    
    if (btn(4) == 0x80)     // down
    {
        direction = 0x08;
    }
    
    switch (direction) {
        case 0x01:
            if (snakebody1[0].x >= 123) {
                wallcollision = true;
            }
            StraightRight1();
            break;
        case 0x02:
            if (snakebody1[0].x <= 0) {
                wallcollision = true;
            }
            StraightLeft();
            break;
        case 0x04:
            if (snakebody1[0].y <= 0) {
                wallcollision = true;
            }
            StraightUp();
            break;
        case 0x08:
            if (snakebody1[0].y >= 29) {
                wallcollision = true;
            }
            StraightDown();
            break;
    }
    
    snakeHistory1();
    playing = !collision(snakelength) && !wallcollision && !hardCollide();    //stops game loop after collisions
    printsnake1(snakelength);
    
    printapple(applex,appley);
    //prints apple on screen
    hardMAP();
    //prints obstacles for hard game mode
    updateDisp();
    cleardisp();
    
}
cleardisp();
updateDisp();
return snakelength;
}

int snakeEASY( void ) { //Moritz

snakebodyINIT();
spi_PowerOnDisplay();
createHead1();

int snakelength = 16;
bool playing = true;
bool wallcollision = false;
bool ateapple = false;
char direction = 0x01;

char applex = 28;
char appley = 13;

lightdisplay();
    time(5000);
    time(5000);
    
while(playing) {
    //main game loop
    
    if ((snakebody1[0].x > applex -2 ) && (snakebody1[0].x < applex + 2))           // checks the overlap between head and apple
    {
        if ((snakebody1[0].y > appley-2) && (snakebody1[0].y < appley + 2)) {
            ateapple = true;
        }
    }
    
    if (ateapple) {
        // checks if the apple has been eaten or not
        applex = randintx(snakelength);
        appley = randinty(snakelength);
        snakelength += 8;
        ateapple = false;
    }
    
    if (btn(1) == 0x2)      // right
    {
        direction = 0x01;
    }
    
    if (btn(2) == 0x20)     // left
    {
        direction = 0x02;
    }
    
    if (btn(3) == 0x40)     // up
    {
        direction = 0x04;
    }
    
    if (btn(4) == 0x80)     // down
    {
        direction = 0x08;
    }
    
    switch (direction) {
        case 0x01:
            StraightRight1();
            if (snakebody1[0].x >= 123) {
                wallcollision = true;
            }
            
            break;
        case 0x02:
            StraightLeft();
            if (snakebody1[0].x <= 0) {
                wallcollision = true;
            }
            
            break;
        case 0x04:
            StraightUp();
            if (snakebody1[0].y <= 0) {
                wallcollision = true;
            }
            
            break;
        case 0x08:
            StraightDown();
            if (snakebody1[0].y >= 29) {
                wallcollision = true;
            }
            
            break;
    }
    
    snakeHistory1();
    playing = !collision(snakelength) && !wallcollision;
    //stops game loop after collisions
    printsnake1(snakelength);
    
    printapple(applex,appley);
    //prints apple on screen
    
    updateDisp();
    cleardisp();
    
}
cleardisp();
updateDisp();
return snakelength;
}

int AIEasy(char dif) {   //Moritz
    time(5000);
    time(5000);
    
    snakebodyINIT();
    BotINIT();
    spi_PowerOnDisplay();
    createHead1();
    createBotHead();
    
    
    int snakelength = 16;
    int Botlength = 16;
    bool playing = true;
    bool wallcollision = false;
    bool ateapple = false;
    bool botAte = false;
    bool snakeAte = false;
    int del = 0;
    int difficulty = dif;
    
    char direction = 0x01;
    char applex = 29;
    char appley = 14;
    
    
    lightdisplay();
    while(playing) {
        //main game loop
        
        if ((snakebody1[0].x > applex -2 ) && (snakebody1[0].x < applex + 2))           // checks the overlap between head and apple
        {
            if ((snakebody1[0].y > appley-2) && (snakebody1[0].y < appley + 2)) {
                ateapple = true;
                snakeAte = true;
                
            }
        }
        if ((Botsnake[0].x >= applex -2 ) && (Botsnake[0].x <= applex + 2)) {           // checks the overlap between head and apple for the AI snake
            
            if ((Botsnake[0].y >= appley-2) && (Botsnake[0].y <= appley + 2)) {
                ateapple = true;
                botAte = true;
            }
        }
        if (ateapple) {
            //checks if the apple has been eaten or not
            applex = randintx(snakelength);
            appley = randinty(snakelength);
            ateapple = false;
        }
        
        if (snakeAte) {
            //checks whether the snake has eaten the apple, in which case it grows
            snakeAte = false;
            snakelength += 8;
        }
        
        if (botAte) {
            //checks whether the AI snake has eaten the apple, in which case it grows
            botAte = false;
            Botlength += 4;
        }
        
        if (btn(1) == 0x2)      // right
        {
            direction = 0x01;
        }
        
        if (btn(2) == 0x20)     // left
        {
            direction = 0x02;
        }
        
        if (btn(3) == 0x40)     // up
        {
            direction = 0x04;
        }
        
        if (btn(4) == 0x80)     // down
        {
            direction = 0x08;
        }
        
        if(del == difficulty) {
            //to adjust the difficulty of the AI we chose to make delay in movement the factor
            //del is a variable which is iterated in each game loop, difficulty dictates how many iterations are required for the AI snake to move, thereby dragging it by some constant
            if (Botsnake[0].x > applex)
            {
                BotLeft();
            }
            if (Botsnake[0].x < applex)
            {
                BotRight();
            }
            if ((Botsnake[0].y > appley) && (applex == Botsnake[0].x))
            {
                BotUp();
            }
            if ((Botsnake[0].y < appley) && (applex == Botsnake[0].x))
            {
                BotDown();
            }
        }
        
        switch (direction) {
            case 0x01:
                StraightRight1();
                if (snakebody1[0].x >= 123) {
                    wallcollision = true;
                }
                
                break;
            case 0x02:
                StraightLeft();
                if (snakebody1[0].x <= 0) {
                    wallcollision = true;
                }
                
                break;
            case 0x04:
                StraightUp();
                if (snakebody1[0].y <= 0) {
                    wallcollision = true;
                }
                
                break;
            case 0x08:
                StraightDown();
                if (snakebody1[0].y >= 29) {
                    wallcollision = true;
                }
                
                break;
        }
        
        snakeHistory1();
        if(del == difficulty) {
            //shifts the botsnake data to the right
            BotHistoryEasy();
        }
        if(del == difficulty) {
            //resets drag constant
            del = 0;
        }
        del += 1;
        playing = !collision(snakelength) && !wallcollision && !BotCollision(Botlength);
        //stops game loop after collisions
        printBot(Botlength);
        printsnake1(snakelength);
        //prints both the player snake and the AI snake
        
        printapple(applex,appley);
        //prints apple on screen
       
        updateDisp();
        cleardisp();
        
    }

cleardisp();
updateDisp();
return snakelength;
}
