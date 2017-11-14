
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

#define BACKLIGHT 11

int lcd_led = BACKLIGHT;
char duty = 128;
DateTime now, then;

RTC_DS1307 rtc;

byte a[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000
};

byte b[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000
};

byte c[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100
};

byte d[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110
};

byte e[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
};

byte f[8] = {
    B01111,
    B01111,
    B01111,
    B01111,
    B01111,
    B01111,
    B01111,
    B01111
};

byte g[8] = {
    B00111,
    B00111,
    B00111,
    B00111,
    B00111,
    B00111,
    B00111,
    B00111
};

byte h[8] = {
    B00011,
    B00011,
    B00011,
    B00011,
    B00011,
    B00011,
    B00011,
    B00011
};

const int rs = 12, en = 3, d4 = 5, d5 = 4, d6 = 6, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void write_day_len_bar(float len_day) {
    float percent_day = len_day/24.0;
    float day_blocks = 8*percent_day;
    int full_blocks = (int)day_blocks;
    float partial_blocks = day_blocks - full_blocks;
    byte left, right;

    //Rewrite using mod
    if (partial_blocks < .2) {
        left = 0;
        right = 7;
    } else if (partial_blocks >= .25 && partial_blocks < .5) {
        left = 1;
        right = 6;
    } else if (partial_blocks >= .5 && partial_blocks < .75) {
        left = 2;
        right = 5;
    } else if (partial_blocks >= .75 && partial_blocks < 1) {
        left = 2;
        right = 4;
    }

    lcd.setCursor(6-(full_blocks/2),0);
    lcd.write(left);
    for (int i = 0; i < full_blocks*2; i++) {
        lcd.write(byte(3));
    }
    lcd.write(right);

    //DEBUG
    lcd.setCursor(11,1);
    lcd.print(partial_blocks);
}

void write_clock(int col, int row, int t) {
    lcd.setCursor(col,row);
    if (t < 10) {
        lcd.print('0');
    }
    lcd.print(t);
}

void update_hour(int hour) {
    write_clock(0, 1, hour);
}

void update_minute(int minute) {
    write_clock(3, 1, minute);
}

void update_second(int second) {
    write_clock(6,1, second);
}

float daylen = 0;//8.783;

void setup() {
    lcd.createChar(0,h);
    lcd.createChar(1,g);
    lcd.createChar(2,f);
    lcd.createChar(3,e);
    lcd.createChar(4,d);
    lcd.createChar(5,c);
    lcd.createChar(6,b);
    lcd.createChar(7,a);
    Wire.begin();
    rtc.begin();
    lcd.begin(16,2);
    //rtc.adjust(DateTime(2017, 11, 12, 12, 18, 0));
    pinMode(lcd_led, OUTPUT);
    lcd.setCursor(2,1);
    lcd.write(":  :");

}

void loop() {
    write_day_len_bar(daylen);
    then = now;
    now = rtc.now();
    analogWrite(lcd_led, duty);

    if (then.hour() != now.hour()) {
        update_hour(now.hour());
    }

    if (then.minute() != now.minute()) {
        update_minute(now.minute());
    }

    if (then.second() != now.second()) {
        update_second(now.second());
        daylen = daylen + 0.1;
        if (daylen > 24) {
            daylen = 0;
        }
    }
    lcd.display();
}
