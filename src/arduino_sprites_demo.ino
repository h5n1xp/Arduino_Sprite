//
//  arduino_sprites_demo.cpp
//  Arduino_Sprite
//
//  Created by Matt Parsons on 27/01/2023.
//  Copyright © 2023 Matt Parsons. All rights reserved.
//


#include "Arduino.h"
#include "Arduino_GFX_Library.h"
#include "pin_config.h"

#include "karamoon2i.h"
#include "fore3i.h"
#include "aarbron2i.h"


//#include "arduino_sprite.h"
#include "arduino_sprite_palette.h"

#include <stdlib.h>

Arduino_DataBus *bus = new Arduino_ESP32LCD8(7 /* DC */, 6 /* CS */, 8 /* WR */, 9 /* RD */, 39 /* D0 */, 40 /* D1 */, 41 /* D2 */, 42 /* D3 */,
                                             45 /* D4 */, 46 /* D5 */, 47 /* D6 */, 48 /* D7 */);
Arduino_GFX *display = new Arduino_ST7789(bus, 5 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */,
                                      0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);




Arduino_GFX* gfx;
Arduino_Sprite_Palette* aarbron;
Arduino_Sprite_Palette* foreground;
Arduino_Sprite_Palette* karamoon;



#define WIDTH 320
#define HEIGHT 170


void setup() {

  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  ledcSetup(0, 2000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
  ledcWrite(0, 0); /* Screen brightness to 0 during set up */


  gfx  = new Arduino_Canvas(WIDTH, HEIGHT, display,0,0);
  gfx->begin(30800000);//(30800000);

  display->setRotation(3);

  gfx->fillScreen(BLUE);   
  gfx->flush();  
  display->fillScreen(RED);
 


  foreground = new Arduino_Sprite_Palette(768,170,gfx); 
  foreground->begin((uint8_t*)imageFore3,paletteFore3);
  foreground->SetChromaKey(0x0);


  aarbron = new Arduino_Sprite_Palette(408,52,gfx);
  aarbron->begin((uint8_t*)imageAarbron2,paletteAabron);
  aarbron->SetChromaKey(0x0);

  karamoon = new Arduino_Sprite_Palette(336,200,gfx);
  karamoon->begin((uint8_t *)imageKaramoon2,paletteKaramoon);
  karamoon->SetChromaKey(0x0);

  ledcWrite(0, 255); /* Screen brightness to maximum) */

}







int aAframe = 0;
int scroll = 0;
int scroll2 = 0;

#define karamoonWidth 336
#define foreWidth 768

void Update(void){
  
  //Background
  scroll -= 2;

  if(scroll < 1 ){
    scroll = (karamoonWidth - 1);
  }



  karamoon->BlitFast(scroll,0,0,0,karamoonWidth - scroll,140);
  karamoon->BlitFast(0,0,karamoonWidth - scroll,0,scroll,140);    




//Main player
  aAframe += 34;

  if(aAframe > 374){
    aAframe = 0;
  }

 aarbron->BlitFastWithKey(143,91,aAframe,0,34,52);


  // Foreground
  scroll2 -= 4; 

  if(scroll2 < 1){
    scroll2 = (foreWidth - 1);
  }


  foreground->BlitFastWithKey(scroll2,0,0,0,foreWidth - scroll2,170);
  foreground->BlitFastWithKey(0,0,foreWidth - scroll2,0,scroll2,170);

}




void loop(void) {
 
  Update();

  gfx->flush();
}