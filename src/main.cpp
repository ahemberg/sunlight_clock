
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

#define BACKLIGHT 11

int lcd_led = BACKLIGHT;
char duty = 60;
DateTime now, then;

RTC_DS1307 rtc;

const int rs = 12, en = 3, d4 = 5, d5 = 4, d6 = 6, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

void setup() {
    Wire.begin();
    rtc.begin();
    //rtc.adjust(DateTime(2017, 11, 12, 12, 18, 0));
    pinMode(lcd_led, OUTPUT);
    lcd.begin(16, 2);
    lcd.setCursor(2,1);
    lcd.write(":  :");
}

void loop() {
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
    }

    lcd.display();
}
