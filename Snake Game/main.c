#include <pic32mx.h>
#include "utils.h"
#include "gamefunc.h"
//FINAL FINAL


void score2char( void ) {   // Viktor
    // updates the characters corresponding to the score of the player
    int i;
    int j;
    int k;
    int l;
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 3; j++){
            for(k = 0; k < 3; k++){
                l  = scoretable[i][j][k].score;
                scoretable[i][j][k].scoreletters[0] = HundredthToChar(l) + 26;
                scoretable[i][j][k].scoreletters[1] = TenthToChar(l) + 26;
                scoretable[i][j][k].scoreletters[2] = OnethToChar(l) + 26;
            }
        }
    }
}

int main() {     // Moritz
    SPI_Config();
    spi_PowerOnDisplay();
    
    struct player player1Easy1;
    struct player player2Easy1;
    struct player player3Easy1;
    struct player player1Medium1;
    struct player player2Medium1;
    struct player player3Medium1;
    struct player player1Hard1;
    struct player player2Hard1;
    struct player player3Hard1;
    struct player player1Easy2;
    struct player player2Easy2;
    struct player player3Easy2;
    struct player player1Medium2;
    struct player player2Medium2;
    struct player player3Medium2;
    struct player player1Hard2;
    struct player player2Hard2;
    struct player player3Hard2;
    //initializes all the sturct objects of type player, each player is associated with a game type, difficulty and is designated a rank. The way the scoreboard works is that the player structs are not swapped around, instead each player is associated with a rank and instead we change the attribute values of the player to aggree with the player initials and scores
    scoretable[0][0][0] = player1Easy1;
    scoretable[0][0][1] = player2Easy1;
    scoretable[0][0][2] = player3Easy1;
    scoretable[0][1][0] = player1Medium1;
    scoretable[0][1][1] = player2Medium1;
    scoretable[0][1][2] = player3Medium1;
    scoretable[0][2][0] = player1Hard1;
    scoretable[0][2][1] = player2Hard1;
    scoretable[0][2][2] = player3Hard1;
    scoretable[1][0][0] = player1Easy2;
    scoretable[1][0][1] = player2Easy2;
    scoretable[1][0][2] = player3Easy2;
    scoretable[1][1][0] = player1Medium2;
    scoretable[1][1][1] = player2Medium2;
    scoretable[1][1][2] = player3Medium2;
    scoretable[1][2][0] = player1Hard2;
    scoretable[1][2][1] = player2Hard2;
    scoretable[1][2][2] = player3Hard2;
    //the player structs are added to the scoretable 3D array for easy access
    
    int i;
    int j;
    int k;
    int l;
    
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 3; j++){
            for(k = 0; k < 3; k++){
                for(l = 0; l < 3; l++) {
                    scoretable[i][j][k].initials[l] = 36;
                    scoretable[i][j][k].scoreletters[l] = 36;
                    scoretable[i][j][k].score = 0;
                }
            }
        }
    }
    //here the struct attributes for each player are reset
    
    bool playing = true;
    char directory;
    int currentscore;
    while(playing) {
    //we create an infinite loop which continuous so long there is power supplied to the chipkit
    //within this loop we have implemented a number of switch statements which select the directories through the menu, based on the selections made by the user.
    //the menu can be traced using the comment tags for each section
        directory = menuselect();
        switch(directory) {
            case(8):
                //scoreboard
                directory = gametypeselect();
                switch(directory){
                    case(10):
                        //difficulty
                        directory = difficultyselect();
                        switch(directory) {
                            case(8):
                                //easy scoreboard (1P)
                                score2char();
                                ScoreBoardScreen(scoretable[0][0][0],scoretable[0][0][1],scoretable[0][0][2]);
                                break;
                            case(16):
                                //medium scoreboard (1P)
                                score2char();
                                ScoreBoardScreen(scoretable[0][1][0],scoretable[0][1][1],scoretable[0][1][2]);
                                break;
                            case(24):
                                //hard scoreboard (1P)
                                score2char();
                                ScoreBoardScreen(scoretable[0][2][0],scoretable[0][2][1],scoretable[0][2][2]);
                                break;
                        }
                        break;
                    case(20):
                        //difficulty
                        directory = difficultyselect();
                        switch(directory) {
                            case(8):
                                //easy scoreboard (2P AI)
                                score2char();
                                ScoreBoardScreen(scoretable[1][0][0],scoretable[1][0][1],scoretable[1][0][2]);
                                break;
                            case(16):
                                //medium scoreboard (2P AI)
                                score2char();
                                ScoreBoardScreen(scoretable[1][1][0],scoretable[1][1][1],scoretable[1][1][2]);
                                break;
                            case(24):
                                //hard scoreboard (2P AI)
                                score2char();
                                ScoreBoardScreen(scoretable[1][2][0],scoretable[1][2][1],scoretable[1][2][2]);
                                break;
                        }
                }
                break;
            case(16):
                //1P
                directory = difficultyselect();
                switch(directory) {
                    case(8):
                        //easy (1P)
                        currentscore = snakeEASY();
                        //here the player attributes are updated based on the resutls of the score of the game
                        if (currentscore > scoretable[0][0][0].score) {
                            scoretable[0][0][2].score = scoretable[0][0][1].score;
                            scoretable[0][0][2].initials[0] = scoretable[0][0][1].initials[0];
                            scoretable[0][0][2].initials[1] = scoretable[0][0][1].initials[1];
                            scoretable[0][0][2].initials[2] = scoretable[0][0][1].initials[2];
                            //shifts leaderboard downwards
                            scoretable[0][0][1].score = scoretable[0][0][0].score;
                            scoretable[0][0][1].initials[0] = scoretable[0][0][0].initials[0];
                            scoretable[0][0][1].initials[1] = scoretable[0][0][0].initials[1];
                            scoretable[0][0][1].initials[2] = scoretable[0][0][0].initials[2];
                            //shifts leaderboard downwards
                            typename(0,0,0);
                            scoretable[0][0][0].score = currentscore;
                        }
                        else if (currentscore > scoretable[0][0][1].score) {
                            scoretable[0][0][2].score = scoretable[0][0][1].score;
                            scoretable[0][0][2].initials[0] = scoretable[0][0][1].initials[0];
                            scoretable[0][0][2].initials[1] = scoretable[0][0][1].initials[1];
                            scoretable[0][0][2].initials[2] = scoretable[0][0][1].initials[2];
                            
                            typename(0,0,1);
                            scoretable[0][0][1].score = currentscore;
                        }
                        else if (currentscore > scoretable[0][0][2].score) {
                            typename(0,0,2);
                            scoretable[0][0][2].score = currentscore;
                        }
                        
                        break;
                    case(16):
                        //medium (1P)
                        currentscore = snakeMEDIUM();
                        if (currentscore > scoretable[0][1][0].score) {
                            scoretable[0][1][2].score = scoretable[0][1][1].score;
                            scoretable[0][1][2].initials[0] = scoretable[0][1][1].initials[0];
                            scoretable[0][1][2].initials[1] = scoretable[0][1][1].initials[1];
                            scoretable[0][1][2].initials[2] = scoretable[0][1][1].initials[2];
                            
                            scoretable[0][1][1].score = scoretable[0][1][0].score;
                            scoretable[0][1][1].initials[0] = scoretable[0][1][0].initials[0];
                            scoretable[0][1][1].initials[1] = scoretable[0][1][0].initials[1];
                            scoretable[0][1][1].initials[2] = scoretable[0][1][0].initials[2];
                            
                            typename(0,1,0);
                            scoretable[0][1][0].score = currentscore;
                        }
                        else if (currentscore > scoretable[0][1][1].score) {
                            scoretable[0][1][2].score = scoretable[0][1][1].score;
                            scoretable[0][1][2].initials[0] = scoretable[0][1][1].initials[0];
                            scoretable[0][1][2].initials[1] = scoretable[0][1][1].initials[1];
                            scoretable[0][1][2].initials[2] = scoretable[0][1][1].initials[2];
                            
                            typename(0,1,1);
                            scoretable[0][1][1].score = currentscore;
                        }
                        else if (currentscore > scoretable[0][1][2].score) {
                            typename(0,1,2);
                            scoretable[0][1][2].score = currentscore;
                        }
                        
                        break;
                    case(24):
                        //hard(1P)
                        currentscore = snakeHARD();
                        if (currentscore > scoretable[0][2][0].score) {
                            scoretable[0][2][2].score = scoretable[0][2][1].score;
                            scoretable[0][2][2].initials[0] = scoretable[0][2][1].initials[0];
                            scoretable[0][2][2].initials[1] = scoretable[0][2][1].initials[1];
                            scoretable[0][2][2].initials[2] = scoretable[0][2][1].initials[2];
                            
                            scoretable[0][2][1].score = scoretable[0][2][0].score;
                            scoretable[0][2][1].initials[0] = scoretable[0][2][0].initials[0];
                            scoretable[0][2][1].initials[1] = scoretable[0][2][0].initials[1];
                            scoretable[0][2][1].initials[2] = scoretable[0][2][0].initials[2];
                            
                            typename(0,2,0);
                            scoretable[0][2][0].score = currentscore;
                        }
                        else if (currentscore > scoretable[0][2][1].score) {
                            scoretable[0][2][2].score = scoretable[0][2][1].score;
                            scoretable[0][2][2].initials[0] = scoretable[0][2][1].initials[0];
                            scoretable[0][2][2].initials[1] = scoretable[0][2][1].initials[1];
                            scoretable[0][2][2].initials[2] = scoretable[0][2][1].initials[2];
                            
                            typename(0,2,1);
                            scoretable[0][2][1].score = currentscore;
                        }
                        else if (currentscore > scoretable[0][2][2].score) {
                            typename(0,2,2);
                            scoretable[0][2][2].score = currentscore;
                        }
                        break;
                }
                break;
            case(24):
                //2P AI
                directory = difficultyselect();
                switch(directory) {
                    case(8):
                        //easy (2P AI)
                        currentscore = AIEasy(4);
                        if (currentscore > scoretable[1][0][0].score) {
                            scoretable[1][0][2].score = scoretable[0][0][1].score;
                            scoretable[1][0][2].initials[0] = scoretable[1][0][1].initials[0];
                            scoretable[1][0][2].initials[1] = scoretable[1][0][1].initials[1];
                            scoretable[1][0][2].initials[2] = scoretable[1][0][1].initials[2];
                            //shifts leaderboard downwards
                            scoretable[1][0][1].score = scoretable[1][0][0].score;
                            scoretable[1][0][1].initials[0] = scoretable[1][0][0].initials[0];
                            scoretable[1][0][1].initials[1] = scoretable[1][0][0].initials[1];
                            scoretable[1][0][1].initials[2] = scoretable[1][0][0].initials[2];
                            //shifts leaderboard downwards
                            
                            typename(1,0,0);
                            scoretable[1][0][0].score = currentscore;
                        }
                        else if (currentscore > scoretable[1][0][1].score) {
                            scoretable[1][0][2].score = scoretable[1][0][1].score;
                            scoretable[1][0][2].initials[0] = scoretable[1][0][1].initials[0];
                            scoretable[1][0][2].initials[1] = scoretable[1][0][1].initials[1];
                            scoretable[1][0][2].initials[2] = scoretable[1][0][1].initials[2];
                            
                            typename(1,0,1);
                            scoretable[1][0][1].score = currentscore;
                        }
                        else if (currentscore > scoretable[1][0][2].score) {
                            typename(1,0,2);
                            scoretable[1][0][2].score = currentscore;
                        }
                        break;
                    case(16):
                        //medium (2P AI)
                        currentscore = AIEasy(3);
                        if (currentscore > scoretable[1][1][0].score) {
                            scoretable[1][1][2].score = scoretable[1][1][1].score;
                            scoretable[1][1][2].initials[0] = scoretable[1][1][1].initials[0];
                            scoretable[1][1][2].initials[1] = scoretable[1][1][1].initials[1];
                            scoretable[1][1][2].initials[2] = scoretable[1][1][1].initials[2];
                            //shifts leaderboard downwards
                            scoretable[1][1][1].score = scoretable[1][1][0].score;
                            scoretable[1][1][1].initials[0] = scoretable[1][1][0].initials[0];
                            scoretable[1][1][1].initials[1] = scoretable[1][1][0].initials[1];
                            scoretable[1][1][1].initials[2] = scoretable[1][1][0].initials[2];
                            
                            typename(1,1,0);
                            scoretable[1][1][0].score = currentscore;
                        }
                        else if (currentscore > scoretable[1][1][1].score) {
                            scoretable[1][1][2].score = scoretable[1][1][1].score;
                            scoretable[1][1][2].initials[0] = scoretable[1][1][1].initials[0];
                            scoretable[1][1][2].initials[1] = scoretable[1][1][1].initials[1];
                            scoretable[1][1][2].initials[2] = scoretable[1][1][1].initials[2];
                            
                            typename(1,1,1);
                            scoretable[1][1][1].score = currentscore;
                        }
                        else if (currentscore > scoretable[1][1][2].score) {
                            typename(1,1,2);
                            scoretable[1][1][2].score = currentscore;
                        }
                        break;
                    case(24):
                        //hard (2P AI)
                        currentscore = AIEasy(2);
                        if (currentscore > scoretable[1][2][0].score) {
                            scoretable[1][2][2].score = scoretable[1][2][1].score;
                            scoretable[1][2][2].initials[0] = scoretable[1][2][1].initials[0];
                            scoretable[1][2][2].initials[1] = scoretable[1][2][1].initials[1];
                            scoretable[1][2][2].initials[2] = scoretable[1][2][1].initials[2];
                            //shifts leaderboard downwards
                            scoretable[1][2][1].score = scoretable[1][2][0].score;
                            scoretable[1][2][1].initials[0] = scoretable[1][2][0].initials[0];
                            scoretable[1][2][1].initials[1] = scoretable[1][2][0].initials[1];
                            scoretable[1][2][1].initials[2] = scoretable[1][2][0].initials[2];
                            
                            typename(1,2,0);
                            scoretable[1][2][0].score = currentscore;
                        }
                        else if (currentscore > scoretable[1][2][1].score) {
                            typename(1,2,1);
                            scoretable[1][2][1].score = currentscore;
                        }
                        else if (currentscore > scoretable[1][2][2].score) {
                            typename(1,2,2);
                            scoretable[1][2][2].score = currentscore;
                        }
                        break;
                        
                        break;
                }
        }
    }
    return 0;
}





