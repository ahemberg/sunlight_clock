
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

int lcd_led = LCD_LIGHT_PIN;
char duty = 0;


LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LcdController lcc = LcdController(&lcd,  LCD_WIDTH, LCD_HEIGHT);
DateTime now, then;
RTC_DS1307 rtc;

float daylen = 8.783;
int pinval = LOW;
unsigned long timer_a = 0;
bool dimming = false;

void setup() {
    Wire.begin();
    rtc.begin();
    lcc.initialize_display();
    pinMode(lcd_led, OUTPUT);
    pinMode(BUTTON_1, INPUT);
    analogWrite(lcd_led, duty);
}

void loop() {
    daylen = 18.4;
    then = now;
    now = rtc.now();
    analogWrite(lcd_led, duty);

    pinval = digitalRead(BUTTON_1);

    if (pinval == HIGH) {
        duty = 255;
        timer_a = millis();
        dimming = false;
    }
    if (then.hour() != now.hour()) {
        lcc.update_hour(now.hour());
        lcc.write_day_len_bar(daylen);
    }
    if (then.minute() != now.minute()) {
        lcc.update_minute(now.minute());
    }
    if (then.second() != now.second()) {
        lcc.update_second(now.second());
    }

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
