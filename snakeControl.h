#ifndef SNAKE_CONTROL_H
#define SNAKE_CONTROL_H

#include "mbed.h"
#include "FXOS8700Q.h"


class snakeControl {
    
    public:
    snakeControl(void);
    int getDirection(void);
    
    
    private:
    
    void pollAccel(void);
    
    //Thread poll;
    int direction;
    motion_data_units_t acc_data;
    float faX, faY, tmp_float;

};

#endif