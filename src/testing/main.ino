//
//  arduino_sprites_demo.cpp
//  Arduino_Sprite
//
//  Created by Matt Parsons on 27/01/2023.
//  Copyright © 2023 Matt Parsons. All rights reserved.
//

#include <stdlib.h>
#include "Arduino.h"
#include "Arduino_GFX_Library.h"
#include "pin_config.h"

#define DISPLAY_HEIGHT 170
#define DISPLAY_WIDTH 320


#include "testing/testing.h"


Arduino_DataBus *bus = new Arduino_ESP32LCD8(7 /* DC */, 6 /* CS */, 8 /* WR */, 9 /* RD */, 39 /* D0 */, 40 /* D1 */, 41 /* D2 */, 42 /* D3 */,
                                             45 /* D4 */, 46 /* D5 */, 47 /* D6 */, 48 /* D7 */);
Arduino_GFX *display = new Arduino_ST7789(bus, 5 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */,
                                      0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

Arduino_GFX* gfx;



int frame = 0;
volatile int fSnapShot = 0;
TaskHandle_t SecondTicker;
void SecondTickerCode(void* p){

  //Infinite Loop
  for(;;){

    //Sleep for 1 second
    sleep(1);
    fSnapShot = frame;
    frame = 0;


  }

}




void setup() {

  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  ledcSetup(0, 2000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
  ledcWrite(0, 0); /* Screen brightness to 0 during set up */


  gfx  = new Arduino_Canvas(DISPLAY_WIDTH, DISPLAY_HEIGHT, display,0,0);
  gfx->begin();

  display->setRotation(3);
  display->invertDisplay(false);

  gfx->fillScreen(RED);  
  gfx->flush();  
  
  testingSetup(gfx);

  ledcWrite(0, 250); /* Screen brightness to almost maximum) */


  gfx->setTextColor(WHITE, BLACK);
  gfx->setTextSize(1);
  gfx->setCursor(2, 2);

 xTaskCreatePinnedToCore(SecondTickerCode,"Seconds",1000,NULL,-1,&SecondTicker,0);
}






void loop(void) {
 
  testingLoop();

  gfx->print(fSnapShot);
  gfx->setCursor(2, 2);

  gfx->flush();
  frame++;
}