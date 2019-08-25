#include "snakeGame.h"


#define NULL 0


snakeGame::snakeGame(int startingDir)
{
    head = (SEG*)malloc(sizeof*head);

    food = (SEG*)malloc(sizeof*food);

    inPlay = 1;
    length = 1;
    currentDir = startingDir;
    head->x = START_X;
    head->y = START_Y;
    head->next = NULL;
    head->prev = NULL;

    food->x = START_X;
    food->y = START_Y;
    food->next = NULL;
    food->prev = NULL;

    tail = head;
    wait_ms(rand()%100);
    newFood();
    //refresh.attach(callback(this, &snakeGame::drawBoard), 0.1);
}

bool snakeGame::checkCollision(int x, int y, SEG* head)
{
    SEG* p = head;
    while(p!=NULL) {
        if(p->x == x && p->y == y) return true;
        p = p->next;
    }
    return false;
}

void snakeGame::newFood(void)
{
    int x = 0;
    int y = 0;
    do {
        srand(time(0));
        x = (int)(rand() % 32);
        srand(time(0)+1);
        y = (int)(rand() % 32);
    } while(checkCollision(x,y,head) && length < 1024);
    food->x = x;
    food->y = y;     //place food on map
}

void snakeGame::updateSnake(int dir)
{
    if(inPlay) {
        SEG* next = (SEG*)malloc(sizeof*next);
        int currx = head->x;
        int curry = head->y;

        switch(dir) {
            case UP :
                if(currentDir != DOWN) {
                    next->x = currx;
                    next->y = curry + 1;
                    currentDir = dir;
                }
                break;
            case DOWN :
                if(currentDir != UP) {
                    next->x = currx;
                    next->y = curry - 1;
                    currentDir = dir;
                }
                break;
            case LEFT :
                if(currentDir != RIGHT) {
                    next->x = currx + 1;
                    next->y = curry;
                    currentDir = dir;
                }
                break;
            case RIGHT :
                if(currentDir != LEFT) {
                    next->x = currx - 1;
                    next->y = curry;
                    currentDir = dir;
                }
                break;
            default :
                updateSnake(currentDir);
                break;
        }


        //first check collisions
        currx = next->x;
        curry = next->y;
        if(currx > WIDTH - 1 || currx < 0 || curry > HEIGHT - 1 || curry < 0 || checkCollision(currx, curry, head)) {
            gameOver();
        }

        //update snake
        next->next = head;
        next->prev = NULL;
        head->prev = next;
        head = next;

        //check food for tail growth
        if(!checkCollision(food->x, food->y, head)) {
            tail = tail->prev;
            tail->next = NULL;
        } else {//otherwise keep tail the same
            length++;
            newFood();
        }
    }
}

SEG* snakeGame::getSnake(void)
{
    return head;
}

SEG* snakeGame::getFood(void)
{
    return food;
}

int snakeGame::getLength(void)
{
    return length;
}

void snakeGame::gameOver(void)
{
    //printf("GAMEOVER!!!");
    inPlay = 0;
    //while(1) {printf("GAME OVER, your score is %06d\r\n", length);};
}

int snakeGame::getInPlay(void)
{
    return inPlay;
}