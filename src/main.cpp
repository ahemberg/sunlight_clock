
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <LcdController.h>
#include <LiquidCrystal.h>

#define LCD_LIGHT_PIN 11
#define LCD_RS 12
#define LCD_EN 3
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 6
#define LCD_D7 8
#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define BUTTON_1 7

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LcdController lcc = LcdController(&lcd,  LCD_WIDTH, LCD_HEIGHT);
DateTime now;
RTC_DS1307 rtc;

float daylen = 8.783;
float old_daylen = 0;
int pinval = LOW;
unsigned long timer_a = 0;
bool dimming = false;
char duty = 0;
int date_now[6];

void setup() {
    Wire.begin();
    rtc.begin();
    lcc.initialize_display();
    lcc.write_day_len_bar(daylen);
    pinMode(LCD_LIGHT_PIN, OUTPUT);
    pinMode(BUTTON_1, INPUT);
    analogWrite(LCD_LIGHT_PIN, duty);
}

void loop() {
    now = rtc.now();
    date_now[0] = now.year();
    date_now[1] = now.month();
    date_now[2] = now.day();
    date_now[3] = now.hour();
    date_now[4] = now.minute();
    date_now[5] = now.second();
    analogWrite(LCD_LIGHT_PIN, duty);
    pinval = digitalRead(BUTTON_1);

    if (pinval == HIGH) {
        duty = 255;
        timer_a = millis();
        dimming = false;
    }

//    lcc.update_hour(now.hour());
//    lcc.update_minute(now.minute());
//    lcc.update_second(now.second());
    lcc.write_date_row(date_now);
    lcc.update_unixtime(now.unixtime());
    if (daylen != old_daylen) {
        lcc.write_day_len_bar(daylen);
    }
    old_daylen = daylen;


    if (millis() - timer_a > 10000) {
        if (timer_a != 0) {
            dimming = true;
        }
        timer_a = 0;
    }

    if (dimming) {
        duty--;
        delay(10);
    }

    if (duty == 0) {
        dimming = false;
    }
}
