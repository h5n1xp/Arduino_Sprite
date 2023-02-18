//
//  arduino_sprite.h
//  Arduino_Sprite
//
//  Created by Matt Parsons on 27/01/2023.
//  Copyright © 2023 Matt Parsons. All rights reserved.
//


#ifndef _ARDUINO_SPRITE_H_
#define _ARDUINO_SPRITE_H_

#include "Arduino.h"
#include "Arduino_GFX_Library.h"

class Arduino_Sprite : public Arduino_Canvas{
  public:
  Arduino_Sprite(int16_t w, int16_t h, Arduino_GFX *output);

  // proposed begin methods
  void begin(void); //just setup single buffer with width and height
  void begin(uint16_t* source); //setup but link to buffer (must be same width and height as created in constructor)
  void begin(int32_t speed) override; //disable this method

  void SetChromaKey(uint16_t chromaKey);
  void SetBackingStore(void); //Needed to use the non destructive drawing methods

  //Update sprite position but don't redraw.
  void Move(int x, int y);

  int GetX(void);
  int GetY(void);

  //Fast Methods don't save the background
  void DrawFast(void);
  void DrawFast(uint16_t x, uint16_t y);
  void DrawFastWithKey(void);
  void DrawFastWithKey(uint16_t x, uint16_t y);

  //Slow backing stored draw methods.
  void Draw(void);
  void Draw(uint16_t x, uint16_t y);
  void DrawWithKey(void);
  void DrawWithKey(uint16_t x, uint16_t y);

  //Blitter methods
  void BlitFast(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h);
  void BlitFastWithKey(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h);

  //Experimental methods, not for use in projects.
  void BlitFastAligned(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h);
  

  //Backing store
  void Clear(void);


  protected:
  void SaveBackground(uint16_t sx, uint16_t sy);
  
  uint16_t _key;
  uint16_t* _backingStore;
  uint16_t* _destBuffer;
  uint16_t _lineMod;  // number of pixels from one line to the next
  uint16_t _maxY;


};

#endif // _ARDUINO_SPRITE_H_

