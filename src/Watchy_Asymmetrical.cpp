#include "Watchy_Asymmetrical.h"

#include "Lato_Heavy_15.h"
#include "Lato_Heavy_115.h"
#include "icons.h"

#define DARKMODE false

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void WatchyAsymm::drawWatchFace() {
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();
//    display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
//    if(BLE_CONFIGURED){
//        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
//    }
}


void WatchyAsymm::drawTime()  {
    int displayHour;
    char hour[3] = {0, 0, 0};
    char minute[3] = {0, 0, 0};
    int16_t curX, curY, x, y;
    uint16_t w, h;

    curX = 0, curY = 0;

    display.setFont(&Lato_Heavy_115);
    display.setCursor(curX, curY);

    if (HOUR_12_24==12) {
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if (displayHour < 10) {
        hour[0] = '0';
        itoa(displayHour, hour+1, 10);
    }
    else {
        itoa(displayHour, hour, 10);
    }
    display.getTextBounds("00", curX, curY, &x, &y, &w, &h);
    Serial.printf("bound %s : cursor %dx%d text %dx%d+%d+%d\n", hour, curX, curY, x, y, w, h);

    // Align the hours in top-half, left 2/3rds of the screen.
    curX = 100 - (w / 4 * 3) - x - 2;
    curY = h + ((100 - h) / 2);
    display.setCursor(curX, curY);
    display.print(hour);
    Serial.printf("print %s : cursor %dx%d text %dx%d+%d+%d\n", hour, curX, curY, x, y, w, h);



    curX = 0;
    curY = 100;
    if(currentTime.Minute < 10){
        minute[0] = '0';
        itoa(currentTime.Minute, minute+1, 10);
    }
    else {
        itoa(currentTime.Minute, minute, 10);
    }
    display.getTextBounds("00", curX, curY, &x, &y, &w, &h);
    // Serial.printf("bound %s : cursor %dx%d text %dx%d+%d+%d\n", minute, curX, curY, x, y, w, h);

    // Align the minutes in bottom-half, right 2/3rds of the screen.
    curX = 100 - (w / 4 * 1) - x + 1;
    curY = 100 + h + ((100 - h) / 2);

    display.setTextColor(GxEPD_BLACK);
    display.setCursor(curX, curY);
    display.print(minute);
    // Serial.printf("print %s : cursor %dx%d text %dx%d+%d+%d\n", minute, curX, curY, x, y, w, h);
}

void WatchyAsymm::drawDate() {
    char date[100] = {0};
    int16_t curX = 0, curY = 120;

    String dayOfWeek = dayStr(currentTime.Wday);
    String month = monthStr(currentTime.Month);

    display.setFont(&Lato_Heavy_15);
    sprintf(date, "%s\n%s\n%d", dayOfWeek.c_str(), month.c_str(), currentTime.Day);

    // int16_t x, y;
    // uint16_t w, h;
    // display.getTextBounds(date, curX, curY, &x, &y, &w, &h);
    // Serial.printf("bound %s : cursor %dx%d text %dx%d+%d+%d\n", date, curX, curY, x, y, w, h);
    // curY = 200 - h;

    display.setCursor(curX, curY);
    display.print(date);
}

void WatchyAsymm::drawSteps() {
    // reset step counter at midnight
    display.setFont(&Lato_Heavy_15);
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    char stepCount[15] = {0};
    sprintf(stepCount, "%d", sensor.getCounter());
    int16_t x, y;
    uint16_t w, h;
    display.getTextBounds(stepCount, 0, 0, &x, &y, &w, &h);
    Serial.printf("steps %s : %dx%d+%d+%d\n", stepCount, x, y, w, h);

    int16_t curY = 100 - h + 3;
    int16_t curX = 200 - w - x;

    display.drawBitmap(200 - 19, 75 - 23, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(curX, curY);
    display.println(stepCount);
}

void WatchyAsymm::drawBattery() {
    int16_t x = 163;
    int16_t y = 0;
    display.drawBitmap(x, y, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(x+5, y+5, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    float maxVBAT = 4.25; // Battery full at 4.25 volts
    float minVBAT = 2.52; // Battery dead at 2.52 volts
    float percent = (VBAT-minVBAT) * 100 / (maxVBAT-minVBAT);
    if (VBAT > 3.8) {        // > 75%
        batteryLevel = 3;
    }
    else if (VBAT > 3.38) {  // > 50%
        batteryLevel = 2;
    }
    else if (VBAT > 2.95) {  // > 25%
        batteryLevel = 1;
    }
    else {
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(x+5 + (batterySegments * BATTERY_SEGMENT_SPACING), y+5, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }

    char vbatStr[10] = {0};
    sprintf(vbatStr, "%2.0f%%%0.2f", percent, VBAT);
    uint16_t w, h;
    display.getTextBounds(vbatStr, 0, 0, &x, &y, &w, &h);
    Serial.printf("steps %s : %dx%d+%d+%d\n", vbatStr, x, y, w, h);

    y = 40;
    x = 200 - w - x;

    display.setCursor(x, y);
    display.println(vbatStr);
}
