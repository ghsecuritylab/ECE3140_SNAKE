#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <math.h>
#include <cstdlib>
//#include <time.h>
#include <ctime>
#include "mbed.h"
#include "RGBmatrixPanel.h"

#define WIDTH 32
#define HEIGHT 32
#define PIXELS 1024
#define UP 1
#define DOWN 0
#define LEFT 3
#define RIGHT 2
#define START_X 16
#define START_Y 16

typedef struct SEG SEG;
struct SEG {
    int x;
    int y;
    SEG *next;
    SEG *prev;
    };
struct BOARD {
    int board[HEIGHT][WIDTH];
    };


class snakeGame
{
    public:
    snakeGame(int startingDir);
    SEG* getSnake(void);
    SEG* getFood(void);
    void updateSnake(int dir);
    int getLength(void);
    int getInPlay(void);
    int inPlay;
    
    private: 
    int currentDir;
    SEG *head;
    SEG *tail;
    SEG *food;
    int length;
       
    bool checkCollision(int x, int y, SEG* head);
    void newFood(void);
    void gameOver(void);
    
    

    
    

    
    Ticker refresh;
};

#endif