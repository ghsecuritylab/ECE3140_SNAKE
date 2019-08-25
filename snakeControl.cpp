#include "mbed.h"
#include "snakeControl.h"


I2C i2c(PTE25, PTE24);
FXOS8700QAccelerometer acc(i2c, FXOS8700CQ_SLAVE_ADDR1);

snakeControl::snakeControl(void)
{
    acc.enable();
    direction = 0;
    //poll.start(callback(this, &snakeControl::pollAccel));
}
void snakeControl::pollAccel(void)
{
    acc.getX(faX);
    acc.getY(faY);
    faX = (acc_data.x + faX + acc.getX(tmp_float))/3;
    faY = (acc_data.y + faY + acc.getY(tmp_float))/3;

    // determine controls
    if (abs(faX) >= 0.25f && abs(faY) >= 0.25f) {
        if (abs(faX) >= abs(faY)) {
            if (faX > 0) {
                //right();
                direction = 3;
                //printf("Direction changed to RIGHT\r\n");
            } else if (faX < 0) {
                //left();
                direction = 2;
                //printf("Direction changed to LEFT\r\n");
            }
        } else if (abs(faX) < abs(faY)) {
            if (faY > 0) {
                //up();
                direction = 0;
                //printf("Direction changed to UP\r\n");
            } else if (faY < 0) {
                //down();
                direction = 1;
                //printf("Direction changed to DOWN\r\n");
            }
        }
    } else if (faX >= 0.25f) {
        // right();
        direction = 3;
        //printf("Direction changed to RIGHT\r\n");
    } else if (faY >= 0.25f) {
        //up();
        direction = 0;
        //printf("Direction changed to UP\r\n");
    } else {
        if(-faX >= 0.25f) {
            //left();
            direction = 2;
            //printf("Direction changed to LEFT\r\n");
        } else if(-faY >= 0.25f) {
            //down();
            direction = 1;
            //printf("Direction changed to DOWN\r\n");
        }
        // if no registered tilt, do nothing
    }
}
int snakeControl::getDirection(void)
{
    pollAccel();
    return direction;
}



