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

enum BlitterMode{
  MODE_BYTESWAP = 1 << 0,
  MODE_RLE      = 1 << 1
};

enum SpriteFlags{
  SPRITE_HIDDEN      = 1 << 0,
  SPRITE_CHROMAKEY   = 1 << 1,
  SPRITE_SCALED      = 1 << 2,
  SPRITE_DRAWN       = 1 << 3,
};

class node_t{
public:
    node_t* next;
    node_t* prev;
    int16_t priority;
    uint32_t flags;
};

class list_t{
    node_t* head;
};





class Arduino_Sprite : public node_t{
  public:
  Arduino_Sprite(int16_t w, int16_t h, Arduino_GFX* output); // set for the size of the whole sprite image

  // begin methods
  Arduino_Canvas* begin(void); // use the returned Canvas for drawing operations.
  void begin(Arduino_Canvas* canvas); //Provide your own canvas must be the same dimentions as the sprite
  void begin(uint8_t* source, uint16_t* pal); //setup but link to an 8bit buffer (must be same width and height as created in constructor)
  void begin(uint16_t* source); //setup but link to a 16bit buffer
    
  void begin(uint16_t* source, int blitterMode); //Setup link to 16bit buffer, set blitter mode: MODE_BYTESWAP
    
  void initAnim(uint16_t count, int x, int y, int w, int h); // Define a detail of the sprite to be used as an amimation frame
  void SetChromaKey(uint16_t chromaKey); //Set transparent colour
  void SetBackingStore(void); //Needed to use the non destructive drawing methods
  void SetHandle(int x, int y); //Set sprite handle.
  void Hidden(bool state); // Set sprite hidden state, draw operations will do nothing while true.
    
  void Frame(uint16_t frame);   //Set current sprite frame
  void Move(int16_t x, int16_t y); // set sprite position
  void Move(int16_t x, int16_t y, uint16_t frame); // set sprite position and frame
    
  void Scale(float w, float h); // Set sprite (between 1 and 0, i.e. 0.5 = half size)
  void ScaleOff(void); // switch off scaling, if called after Scale() can improve performance if scaling no longer needed.
    
  int GetX(void);   // Get current sprite X position
  int GetY(void);   // Get current sprite Y position.
  int GetFrame(void); // Get current frame
  int GetFrames(void); // Get the total number of frames available in the sprite sheet.
  Arduino_Canvas* GetCanvas(void);// Always check if NULL, as not all sprites have a backing canvas.
    
  void Clear(void); // Clear sprite from display, needs BackingStore() set or this does nothing.
  void Draw(void);  // Draw sprite to screen.
  void Scroll(void); //draw a background, with pixel wrap.
    
  protected:
    
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
    
  //Slower backing stored draw methods, preserve display state

  void Draw(uint16_t frame);
  void Draw(int16_t x, int16_t y);
  void Draw(int16_t x, int16_t y, uint16_t frame);
    
  void DrawWithKey(void);
  void DrawWithKey(uint16_t frame);
  void DrawWithKey(int16_t x, int16_t y);
  void DrawWithKey(int16_t x, int16_t y, uint16_t frame);
    
    //Transform methods
    void DrawFastScaled();
    void DrawFastWithKeyScaled();
    

  //Backing store

  void SaveBackground(int16_t sx, int16_t sy);

    
  int _output_x;
  int _output_y;
    
  blitter_t* _blitter;
    
  Arduino_Canvas* _canvas;
  Arduino_GFX* _output;

    
  uint16_t* _backingStore;

  //Animation
  uint16_t _frames;
  int _frame;
  uint16_t _fx;
  uint16_t _fy;
  uint16_t _fw;
  uint16_t _fh;
    
  float _scaleX;
  float _scaleY;
    
    int16_t _hx;
    int16_t _hy;
    
};


class Arduino_FrameBuffer : public Arduino_Canvas{
public:
    
    Arduino_FrameBuffer(uint16_t w, uint16_t h, Arduino_GFX* output, uint16_t x, uint16_t y) : Arduino_Canvas(w,h,output,x,y){
        
    }
    
   // void Attach(Arduino_Sprite* sprite);
   // void Update(void);
    
   uint16_t readPixel(int16_t x,int16_t y);
   void WritePixel(int16_t x, int16_t y, uint16_t colour);
protected:
    list_t* spriteList;
};

#endif // _ARDUINO_SPRITE_H_

