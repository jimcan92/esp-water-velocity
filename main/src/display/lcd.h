#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern int velocityStatus;
extern float velocity;

class LCD
{
public:
    LCD();
    void init();
    LiquidCrystal_I2C lcd;
};

#endif