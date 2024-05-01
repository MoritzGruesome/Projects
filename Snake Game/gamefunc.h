#include <stdint.h>
#include <stdlib.h>

struct snakepart {          //creates the an "object type" for each snake body part, each piece of the snake contains x,y
    char x;
    char y;
};

struct player {
    char score;
    char initials[3];
    char scoreletters[3];
};



struct player scoretable[2][3][3];
struct snakepart snakebody1[256];
struct snakepart Botsnake[256];

void snakebodyINIT();

void BotINIT( void );

void printsquare(char cornerx, char cornery, char length);

void printsnake1(int length);

void printBot(int length);

void cleardisp( void );

void createHead1( void );

void createBotHead( void );

void StraightRight1( void );

void StraightLeft( void );

void StraightUp( void );

void StraightDown( void );

void BotRight( void );

void BotLeft( void );

void BotUp( void );

void BotDown( void );

void snakeHistory1( void );

void BotHistoryEasy( void);

bool collision(int length );

char randintx( int length );

char randinty( int length );

void writeletter(int x, int y, int index);

void keyboard( void );

void lightdisplay( void );

void mediumMAP( void );

int snakeEASY( void );

bool mediumCollide();

void hardMAP( void );

bool hardCollide();

char randomMediumx(int length, char y);

char randomHardx(int length, char y);

int snakeMEDIUM( void );

int snakeEASY( void );

void menuScreen( void );

void difficultyScreen( void );

void gameTypeScreen( void );

char HundredthToChar( int num);

char TenthToChar(int num);

char OnethToChar(int num);

void ScoreBoardScreen(struct player p1, struct player p2, struct player p3);

char menuselect( void );

char difficultyselect( void );

char gametypeselect( void );

bool BotCollision(int length );

