#include "display/lcd.h"

LCD::LCD() : lcd(0x27, 16, 2) {}

void lcdTask(void *pvParameters);

void LCD::init()
{
    lcd.init();
    lcd.backlight();

    xTaskCreatePinnedToCore(lcdTask, "LCD TASK", 4000, this, 1, NULL, 1);
}

void lcdTask(void *p)
{
    LCD *lcd = (LCD *)p;

    while (true)
    {
        lcd->lcd.setCursor(0, 0);
        lcd->lcd.print("Vel: " + String(velocity) + " m/s          ");
        lcd->lcd.setCursor(0, 1);

        switch (velocityStatus)
        {
        case 1:
            lcd->lcd.print("Warning         ");
            break;
        case 2:
            lcd->lcd.print("Critical        ");
            break;
        case 0:
        default:
            lcd->lcd.print("Normal          ");
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}