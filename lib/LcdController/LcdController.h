#ifndef LcdController_h
#define LcdController_h

#include <LiquidCrystal.h>
#include <Arduino.h>

class LcdController {
    public:
        LcdController(LiquidCrystal* _p_lcd, int columns, int rows);
        void initialize_display();
        void update_hour(int hour);
        void update_minute(int minute);
        void update_second(int second);
        void write_day_len_bar(float len_day);
    protected:
    private:
        LiquidCrystal* p_lcd;
        int columns;
        int rows;
        void write_clock(int col, int row, int t);

        const byte right_one[8] = {
            B10000,
            B10000,
            B10000,
            B10000,
            B10000,
            B10000,
            B10000,
            B10000
        };

        const byte right_two[8] = {
            B10000,
            B11000,
            B11000,
            B11000,
            B11000,
            B11000,
            B11000,
            B10000
        };

        const byte right_three[8] = {
            B10000,
            B11000,
            B11100,
            B11100,
            B11100,
            B11100,
            B11000,
            B10000
        };

        const byte right_four[8] = {
            B10000,
            B11000,
            B11100,
            B11110,
            B11110,
            B11100,
            B11000,
            B10000
        };

        const byte full[8] = {
            B11111,
            B11111,
            B11111,
            B11111,
            B11111,
            B11111,
            B11111,
            B11111
        };

        const byte left_four[8] = {
            B00001,
            B00011,
            B00111,
            B01111,
            B01111,
            B00111,
            B00011,
            B00001
        };

        const byte left_three[8] = {
            B00001,
            B00011,
            B00111,
            B00111,
            B00111,
            B00111,
            B00011,
            B00001
        };

        const byte left_two[8] = {
            B00001,
            B00011,
            B00011,
            B00011,
            B00011,
            B00011,
            B00011,
            B00001
        };
};

#endif
