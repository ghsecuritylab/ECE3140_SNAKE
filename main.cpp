#include "mbed.h"
#include "FXOS8700Q.h"
#include "RGBmatrixPanel.h"
#include "PinNames.h"
#include "snakeControl.h"
#include "snakeGame.h"

#include "fsl_gpio.h"
#include "fsl_port.h"

#define NULL 0

static void setP(PinName p, int value)
{
    uint32_t port = p >> GPIO_PORT_SHIFT;
    uint32_t pin = p & 0xFF;
    uint8_t val;
    if(value == 0) {
        gpio_addrs[port]->PCOR = 1U << pin;
    } else {
        gpio_addrs[port]->PSOR = 1U << pin;
    }

}

static void toggle(void)
{
    PinName p = LED1;
    uint32_t port = p >> GPIO_PORT_SHIFT;
    uint32_t pin = p & 0xFF;
    gpio_addrs[port]->PTOR = 1U << pin;
}

Serial pc(USBTX, USBRX);
PinName lb2=D7;
PinName lg2=D6;
PinName lr2=D5;
PinName ub1=D4;
PinName ug1=D3;
PinName ur1=D2;
RGBmatrixPanel matrix(ur1, lr2, ug1, lg2, ub1, lb2, A0, A1, A2, A3, D8, D10, D9);
snakeGame game(0);
int direction = 0;
Ticker tick;
//tick.attach(callback(&toggle), 0.5);
Ticker drawer;

void setupPins(void)
{
    DigitalOut r1(D2);
    DigitalOut r2(D5);
    DigitalOut g1(D3);
    DigitalOut g2(D6);
    DigitalOut b1(D4);
    DigitalOut b2(D7);
    DigitalOut a(A0);
    DigitalOut b(A1);
    DigitalOut c(A2);
    DigitalOut d(A3);
    DigitalOut clk(D8);
    DigitalOut oe(D9);
    DigitalOut latch(D10);
}

void checkGameOver(void)
{
    if(game.inPlay == 0) {
        drawer.detach();
        for(int i = 0; i < 32; i++) {
            for(int j = 0; j < 32; j++) {
                matrix.drawPixel(j,i,matrix.ColorHSV((i)*5.0f,(j)*5.0f,(i+j)*5.0f,false));
                wait_ms(1);
            }
        }
        //matrix.fillScreen(matrix.Color333(0,0,0));
//        wait_ms(500);
//        matrix.fillScreen(matrix.Color333(7,7,7));
//        wait_ms(500);
//        matrix.fillScreen(matrix.Color333(0,0,0));
//        wait_ms(500);
          // start at top left, with one pixel of spacing
        matrix.setTextSize(1);    // size 1 == 8 pixels high

        int count = 5;
        // GAME OVER
        while(count > 0) {
            matrix.setCursor(4, 0); 
            matrix.fillScreen(matrix.Color333(0,0,0));
            wait(0.5);
            matrix.setTextColor(matrix.Color333(0,0,7));
            matrix.putc('G');
            matrix.putc('A');
            matrix.putc('M');
            matrix.putc('E');
            matrix.setCursor(4, 8);
            matrix.putc('O');
            matrix.putc('V');
            matrix.putc('E');
            matrix.putc('R');
            wait(0.5);
            count--;
        }

        matrix.setCursor(2, 16);
        matrix.setTextColor(matrix.Color333(7,7,7));
        matrix.putc('S');
        matrix.putc('C');
        matrix.putc('O');
        matrix.putc('R');
        matrix.putc('E');


        int digits = 0;
        for(int i = game.getLength(); i > 0; i = i/10) {
            matrix.setCursor(16 - 6*digits, 24);
            digits++;
            matrix.putc((char) ('0' + i%10));
        }
        while(1) printf("Game Over\r\n");
    }
}

void drawSnake(SEG* head)
{
    SEG* p = head;
    while(p!=NULL) {
        matrix.drawPixel(p->x, p->y, matrix.Color333(0,7,0));
        p = p->next;
    }
}
void drawFood(SEG* food)
{
    matrix.drawPixel(food->x, food->y, matrix.Color333(7,7,7));
}

void draw(void)
{
    matrix.fillScreen(matrix.Color333(0,0,0));
    //game.updateSnake(direction);
    SEG* head = game.getSnake();
    SEG* food = game.getFood();
    drawSnake(head);
    drawFood(food);
}

void intro(void)
{
    matrix.fillScreen(matrix.Color333(7,7,7));
    //wait(5);
    matrix.fillScreen(matrix.Color333(0,0,0));
    wait_ms(500);
    matrix.setCursor(1, 12);   // start at top left, with one pixel of spacing
    matrix.setTextSize(1);    // size 1 == 8 pixels high

    // SNAKE
    matrix.setTextColor(matrix.Color333(0,7,0));
    matrix.putc('S');
    matrix.putc('N');
    matrix.putc('A');
    matrix.putc('K');
    matrix.putc('E');

    wait(2);
    matrix.fillScreen(matrix.Color333(0,0,0));
    wait_ms(500);

    // ready?
    matrix.setCursor(1, 8);   // start at top left, with one pixel of spacing
    matrix.setTextSize(1);    // size 1 == 8 pixels high
    matrix.setTextColor(matrix.Color333(7,7,7));
    matrix.putc('R');
    matrix.putc('E');
    matrix.putc('A');
    matrix.putc('D');
    matrix.putc('Y');
    matrix.setCursor(13, 16);
    matrix.putc('?');

    wait(3);

    // countdown from 3
    matrix.setCursor(13, 12);
    matrix.fillScreen(matrix.Color333(0,0,0));
    matrix.setTextColor(matrix.Color333(7,7,7));
    matrix.putc('3');
    matrix.setCursor(13, 12);
    wait(1);
    matrix.fillScreen(matrix.Color333(0,0,0));
    matrix.setTextColor(matrix.Color333(7,7,7));
    matrix.putc('2');
    matrix.setCursor(13, 12);
    wait(1);
    matrix.fillScreen(matrix.Color333(0,0,0));
    matrix.setTextColor(matrix.Color333(7,7,7));
    matrix.putc('1');
    wait(1);
}

int main(void)
{
    setupPins();
    matrix.begin();
    wait_ms(500);
    matrix.fillScreen(matrix.Color333(0,0,0));
    wait_ms(500);

    matrix.fillScreen(matrix.Color333(7,7,7));
    matrix.fillScreen(matrix.Color333(0,0,0));
    wait_ms(500);

    intro();

    DigitalOut led(LED1);


    drawer.attach_us(callback(&draw), 200);
    snakeControl control;

    int c = 0;

    while (true) {

        direction = control.getDirection();
        if(c == 20) {
            printf("Refreshes: %06d\r\n", matrix.numRefreshes);
            printf("Direction: %06d\r\n", control.getDirection());
            game.updateSnake(direction);
            c = 0;
        }
        c++;
        checkGameOver();
        wait(0.001f); // wait at the end of the loop or after it gets the data?
    }

    printf("Gameover");


    wait(10);

}
