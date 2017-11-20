#include "LcdController.h"

LcdController::LcdController(LiquidCrystal* _p_lcd, int columns, int rows) {
    this->p_lcd = _p_lcd;
    this->columns = columns;
    this->rows = rows;
}

void LcdController::initialize_display() {
    this->p_lcd->createChar(0,this->right_one);
    this->p_lcd->createChar(1,this->right_two);
    this->p_lcd->createChar(2,this->right_three);
    this->p_lcd->createChar(3,this->right_four);
    this->p_lcd->createChar(4,this->full);
    this->p_lcd->createChar(5,this->left_four);
    this->p_lcd->createChar(6,this->left_three);
    this->p_lcd->createChar(7,this->left_two);
    this->p_lcd->begin(this->columns, this->rows);
    this->p_lcd->setCursor(2,1);
    this->p_lcd->write(":  :");
    this->p_lcd->display();
}

void LcdController::write_date_row(int date_arr[6]) {
    int curr = 0;
    if (!this->clock_active) {
        this->p_lcd->setCursor(0, this->clock_row);
        this->p_lcd->print("    -  -     :  :  ");
        this->clock_active = true;
    }
    for (int i = 0; i < 6; i++) {
        if (i != 0) {
            curr = 2+3*i;
        } else {
            curr = 0;
        }
        if (date_arr[i] != this->on_screen_clock[i]) {
            this->p_lcd->setCursor(curr, this->clock_row);
            this->p_lcd->print(date_arr[i]);
            this->on_screen_clock[i] = date_arr[i];
        }
    }
}

void LcdController::write_clock(int col, int row, int t) {
    this->p_lcd->setCursor(col,row);
    if (t < 10) {
        this->p_lcd->print('0');
    }
    this->p_lcd->print(t);
}

void LcdController::update_hour(int hour) {
    if (this->on_screen_clock[3] != hour) {
        this->on_screen_clock[3] = hour;
        write_clock(0, 1, hour);
    }
}

void LcdController::update_minute(int minute) {
    if (this->on_screen_clock[4] != minute) {
        this->on_screen_clock[4] = minute;
        write_clock(3, 1, minute);
    }
}

void LcdController::update_second(int second) {
    if (this->on_screen_clock[5] != second) {
        this->on_screen_clock[5] = second;
        write_clock(6, 1, second);
    }
}

void LcdController::update_unixtime(uint32_t unix_ts) {
    if (this->old_unixtime != unix_ts) {
        this->old_unixtime = unix_ts;
        this->p_lcd->setCursor(0,2);
        this->p_lcd->print(unix_ts);
    }
}

void LcdController::write_day_len_bar(float len_day) {
    int screen_mid = this->columns/2;
    float percent_day = len_day/24.0;
    float day_blocks = screen_mid*percent_day;
    int full_blocks = (int)day_blocks;
    float partial_blocks = day_blocks - full_blocks;

    byte left, right;

    this->p_lcd->setCursor(0,0);
    for (int i = 0; i < this->columns; i++) {
        this->p_lcd->write(" ");
    }

    this->p_lcd->setCursor(screen_mid-(full_blocks),0);
    for (int i = 0; i < full_blocks*2; i++) {
        this->p_lcd->write(4);
        delay(1);
    }

    if (partial_blocks < .33) {
        left = 7;
        right = 1;
    } else if (partial_blocks >= .33 && partial_blocks < .66) {
        left = 6;
        right = 2;
    } else if (partial_blocks >= .66 && partial_blocks < 1) {
        left = 5;
        right = 3;
    }

    this->p_lcd->setCursor((screen_mid-1)-(full_blocks),0);
    this->p_lcd->write(left);
    this->p_lcd->setCursor(screen_mid+(full_blocks),0);
    this->p_lcd->write(right);

    //DEBUG
    this->p_lcd->setCursor(8,3);
    this->p_lcd->print(full_blocks);
    this->p_lcd->setCursor(11,3);
    this->p_lcd->print(partial_blocks);
}
