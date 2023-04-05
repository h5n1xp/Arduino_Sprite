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

#include "blitter.h"

class node_t{
public:
    node_t* next;
    node_t* prev;
    int priority;
};

typedef union {
    uint32_t value;
    struct{
     uint8_t lsb;
     uint8_t mlb;
     uint8_t mub;
     uint8_t msb;
    };
}dat32_t;

class Arduino_Sprite : public node_t{
  public:
  Arduino_Sprite(int16_t w, int16_t h, Arduino_GFX* output);

  // begin methods
  Arduino_Canvas* begin(void); // use the returned Canvas for drawing operations.
  void begin(Arduino_Canvas* canvas); //Provide your own canvas must be the same dimentions as the sprite
  void begin(uint8_t* source, uint16_t* pal); //setup but link to buffer (must be same width and height as created in constructor)
  void begin(uint16_t* source);
    
  void initAnim(uint16_t count, int x, int y, int w, int h);
  void SetChromaKey(uint16_t chromaKey);
  void SetBackingStore(void); //Needed to use the non destructive drawing methods

    
  void Frame(uint16_t frame);
  void Move(int16_t x, int16_t y);
  void Move(int16_t x, int16_t y, uint16_t frame);
    
  int GetX(void);
  int GetY(void);
  int GetFrame(void);
  int GetFrames(void);
  Arduino_Canvas* GetCanvas(void);// Always check if null, as not all sprites have a backing canvas.
    
  //Fast Methods don't save the background
  void DrawFast(void);
  void DrawFast(uint16_t frame);
  void DrawFast(int16_t x, int16_t y);
  void DrawFast(int16_t x, int16_t y, uint16_t frame);
  void ScrollFast(void);
    
  void DrawFastWithKey(void);
  void DrawFastWithKey(uint16_t frame);
  void DrawFastWithKey(int16_t x, int16_t y);
  void DrawFastWithKey(int16_t x, int16_t y, uint16_t frame);
  void ScrollFastWithKey(void);
    
  //Slow backing stored draw methods.
  void Draw(void);
  void Draw(uint16_t frame);
  void Draw(int16_t x, int16_t y);
  void Draw(int16_t x, int16_t y, uint16_t frame);
    
  void DrawWithKey(void);
  void DrawWithKey(uint16_t frame);
  void DrawWithKey(int16_t x, int16_t y);
  void DrawWithKey(int16_t x, int16_t y, uint16_t frame);
    

  //Backing store
  void Clear(void);
  void SaveBackground(int16_t sx, int16_t sy);

    
  protected:
    
  int _output_x;
  int _output_y;
    
  blitter_t* _blitter;
    
  Arduino_Canvas* _canvas;
  Arduino_GFX* _output;

  //uint16_t* _framebuffer;
  //uint16_t _width;
  //uint16_t _height;
    
  uint16_t* _backingStore;

  //Animation
  uint16_t _frames;
  int _frame;
  uint16_t _fx;
  uint16_t _fy;
  uint16_t _fw;
  uint16_t _fh;
};

#endif // _ARDUINO_SPRITE_H_

