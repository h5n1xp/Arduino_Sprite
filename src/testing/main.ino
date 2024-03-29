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

#include "arduino_sprite.h"

#define DISPLAY_HEIGHT 170
#define DISPLAY_WIDTH 320



Arduino_DataBus *bus = new Arduino_ESP32LCD8(7 /* DC */, 6 /* CS */, 8 /* WR */, 9 /* RD */, 39 /* D0 */, 40 /* D1 */, 41 /* D2 */, 42 /* D3 */,
                                             45 /* D4 */, 46 /* D5 */, 47 /* D6 */, 48 /* D7 */);
Arduino_GFX *display = new Arduino_ST7789(bus, 5 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */,
                                      0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

Arduino_GFX* gfx;


//Set Up Basic Loop with FPS counter
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

//**************** Sprite Lib Testing Code

//Setup Assets
#include "karamoon2i.h"
#include "a.h"
#include "l.h"

#include "c0.h"
#include "c1.h"
#include "c2.h"
#include "c3.h"
#include "c4.h"

#include "m.h"
#include "g0.h"
#include "g1.h"
#include "g2.h"
#include "g3.h"
#include "g4.h"
#include "b.h"

Arduino_Sprite* karamoon;
Arduino_Sprite* a;
Arduino_Sprite* l;

Arduino_Sprite* c0;
Arduino_Sprite* c1;
Arduino_Sprite* c2;
Arduino_Sprite* c3;
Arduino_Sprite* c4;

Arduino_Sprite* foreground;

Arduino_Sprite* g0;
Arduino_Sprite* g1;
Arduino_Sprite* g2;
Arduino_Sprite* g3;
Arduino_Sprite* g4;
Arduino_Sprite* b;



void testingSetup(Arduino_GFX* gfx){

karamoon = new Arduino_Sprite(336,200,gfx);
  karamoon->initAnim(1, 0, 0, 336, 140);
  karamoon->begin((uint8_t *)imageKaramoon,paletteKaramoon);
    
    
  l = new Arduino_Sprite(53,53,gfx);
  l->begin((uint8_t*)imageL,paletteL);
  l->SetChromaKey(0x0);

  c0 = new Arduino_Sprite(320,21,gfx);
  c0->begin((uint8_t*)imageC0,paletteC0);
  c0->SetChromaKey(0x6);

  c1 = new Arduino_Sprite(320,40,gfx);
  c1->begin((uint8_t*)imageC1,paletteC1);
  c1->SetChromaKey(0x0);

  c2 = new Arduino_Sprite(320,19,gfx);
  c2->begin((uint8_t*)imageC2,paletteC2);
  c2->SetChromaKey(0x0);

  c3 = new Arduino_Sprite(320,9,gfx);
  c3->begin((uint8_t*)imageC3,paletteC3);
  c3->SetChromaKey(0x0);

  c4 = new Arduino_Sprite(320,6,gfx);
  c4->begin((uint8_t*)imageC4,paletteC4);
  c4->SetChromaKey(0x0);
    
    
    
  foreground = new Arduino_Sprite(320,73,gfx);
  foreground->begin((uint8_t*)imageM,paletteM);
  foreground->SetChromaKey(0x0);
    
    
  g0 = new Arduino_Sprite(320,2,gfx);
  g0->begin((uint8_t*)imageG0,paletteG0);

  g1 = new Arduino_Sprite(320,3,gfx);
  g1->begin((uint8_t*)imageG1,paletteG1);
  
  g2 = new Arduino_Sprite(320,7,gfx);
  g2->begin((uint8_t*)imageG2,paletteG2);

  g3 = new Arduino_Sprite(320,7,gfx);
  g3->begin((uint8_t*)imageG3,paletteG3);

  g4 = new Arduino_Sprite(320,11,gfx);
  g4->begin((uint8_t*)imageG4,paletteG4);

  b = new Arduino_Sprite(320,21,gfx);
  b->begin((uint8_t*)imageB,paletteB);
  b->SetChromaKey(0x0);
    
  a = new Arduino_Sprite(240,50,gfx);
  a->begin((uint8_t*)imageA,paletteA);
  a->SetChromaKey(0x0);
  a->Move(130, 94);
  a->initAnim(5,0,0,40,50);
  a->SetBackingStore();

  return;
}





int aAframe = 0;
int scroll = 0;

int scrollL = 0;

int scrollC0 = 0;
int scrollC1 = 0;
int scrollC2 = 0;
int scrollC3 = 0;
int scrollC4 = 0;

int scrollM = 0;

int scrollG0 = 0;
int scrollG1 = 0;
int scrollG2 = 0;
int scrollG3 = 0;
int scrollG4 = 0;
int scrollB = 0;

#define karamoonWidth 336
#define foreWidth 320

int otherFrame = 0;


void testingLoop(void){
     //Background
    scroll -= 1;

    if(scroll < 0 ){
        scroll = karamoonWidth + scroll;
    }
    
    karamoon->Move(scroll,0);
    karamoon->Scroll();
    
    
    
    
    //L
    l->Move(scrollL,20);
    l->Draw();



    //C0
    scrollC0 -= 7;

    if(scrollC0 < 0){
        scrollC0 = 320 + scrollC0;
    }

    c0->Move(scrollC0,0);
    c0->Scroll();

    
    //C1
    scrollC1 -= 6;

    if(scrollC1 < 0){
        scrollC1 = 320 + scrollC1;
    }

    c1->Move(scrollC1,22);
    c1->Scroll();
    
    //C4
    scrollC4 -= 2;

    if(scrollC4 < 0){
        scrollC4 = 320 + scrollC4;
    }

    c4->Move(scrollC4,90);
    c4->Scroll();
    
    
    
    // Foreground
    scrollM -= 3;

    if(scrollM < 0){
      scrollM = 320 + scrollM;
    }

    foreground->Move(scrollM,67);
    foreground->Scroll();
    
    
    //C3
    scrollC3 -= 4;

    if(scrollC3 < 0){
      scrollC3 = 320 + scrollC3;
    }

    c3->Move(scrollC3,81);
    c3->Scroll();

    //C2
    scrollC2 -= 5;

    if(scrollC2 < 0){
      scrollC2 = 320 + scrollC2;
    }

    c2->Move(scrollC2,62);
    c2->Scroll();
 

    //G0
      scrollG0 -= 3;

      if(scrollG0 < 0){
        scrollG0 = 320 + scrollG0;
      }

      g0->Move(scrollG0,140);
      g0->Scroll();


      //G1
      scrollG1 -= 4;

      if(scrollG1 < 0){
        scrollG1 = 320 + scrollG1;
      }
      g1->Move(scrollG1,142);
      g1->Scroll();

    
      //G2
      scrollG2 -= 5;

      if(scrollG2 < 0){
        scrollG2 = 320 + scrollG2;
      }
      g2->Move(scrollG2,145);
      g2->Scroll();

    
      //G3
        scrollG3 -= 6;

      if(scrollG3 < 0){
          scrollG3 = 320 + scrollG3;
      }

      g3->Move(scrollG3,152);
      g3->Scroll();

    
      //G4
      scrollG4 -= 7;

      if(scrollG4 < 0){
        scrollG4 = 320 + scrollG4;
      }
      g4->Move(scrollG4,159);
      g4->Scroll();


      // B
      scrollB -= 8;

      if(scrollB < 0){
        scrollB = 320 + scrollB;
      }
      b->Move(scrollB,149);
      b->Scroll();

    
    if(otherFrame){

      //L
      scrollL -= 1;

      if(scrollL < -53){
        scrollL = 320;
      }

      //Main player
      aAframe += 1;

      if(aAframe > 5){
        aAframe = 0;
      }
    }
    
    a->Frame(aAframe);
    a->Draw();
    
    otherFrame = 1 - otherFrame;
    return;
}








//**************** Standard Arduino setup and loop functions
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

  //Second Ticker thread, pinned to code 0, while Core 1 runs the sprite code
 xTaskCreatePinnedToCore(SecondTickerCode,"Seconds",1000,NULL,-1,&SecondTicker,0);
}






void loop(void) {
 
  testingLoop();

  gfx->print(fSnapShot);
  gfx->setCursor(2, 2);

  gfx->flush();
  frame++;
}
