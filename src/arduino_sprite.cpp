//
//  arduino_sprite.cpp
//  Arduino_Sprite
//
//  Created by Matt Parsons on 27/01/2023.
//  Copyright © 2023 Matt Parsons. All rights reserved.
//


#include "arduino_sprite.h"


Arduino_Sprite::Arduino_Sprite(int16_t w, int16_t h, Arduino_GFX *output): Arduino_Canvas(w, h, output){

}


void Arduino_Sprite::begin(void){
 // Since there is no framebuffer of the Arduino_G type, and sprites won't really work on them anyway, 
  // cast to an Arduino_Canvas type to use the getFramebuffer() method.
  Arduino_Canvas* c = (Arduino_Canvas*)_output; 
  _destBuffer = c->getFramebuffer();
  _lineMod = c->width();
  _maxY = c->height();
  _backingStore = NULL;
  _framebuffer = NULL;

    size_t s = _width * _height * 2;
#if defined(ESP32)
    if (psramFound())
    {
        _framebuffer = (uint16_t *)ps_malloc(s);
    }
    else
    {
        _framebuffer = (uint16_t *)malloc(s);
        if (!_framebuffer)
        {
            // hack for allocate memory over 63,360 pixels
            s /= 2;
            _framebuffer = (uint16_t *)malloc(s);
            uint16_t *tmp = (uint16_t *)malloc(s);
            UNUSED(tmp);
            log_v("_framebuffer delta: %d", tmp - _framebuffer);
        }
    }
#else
    _framebuffer = (uint16_t *)malloc(s);
#endif
    if (!_framebuffer)
    {
        Serial.println(F("_framebuffer allocation failed."));
    }
  
}


void Arduino_Sprite::begin(uint16_t* source){
    Arduino_Canvas* c = (Arduino_Canvas*)_output; 
  _destBuffer = c->getFramebuffer();
  _lineMod = c->width();
  _maxY = c->height();
  _framebuffer = source;
  _backingStore = NULL;
}

void Arduino_Sprite::begin(int32_t speed){
  //NOP
  //This method has no meaning for sprites
  begin();
}

void Arduino_Sprite::SetBackingStore(void){

    size_t s = _width * _height * 2;

#if defined(ESP32)
    if (psramFound()){
        _backingStore = (uint16_t *)ps_malloc(s);
    }else{
        _backingStore = (uint16_t *)malloc(s);
        if (!_backingStore){
            // hack for allocate memory over 63,360 pixels
            s /= 2;

            _backingStore = (uint16_t *)malloc(s);
            uint16_t *tmp = (uint16_t *)malloc(s);
            UNUSED(tmp);

        }
    }
#else
    _backingStore = (uint16_t *)malloc(s);
#endif
}

void Arduino_Sprite::SetChromaKey(uint16_t chromaKey){
  _key = chromaKey;

  if(_backingStore){
    int i = 0;
    for(int y=0;y<_height;++y){
      for(int x=0;x<_width;++x){

          _backingStore[i++] = _key;
    
      }
    }
  }
}

void Arduino_Sprite::Move(int sx, int sy){
  _output_x = sx;
  _output_y = sy;
}

int Arduino_Sprite::GetX(void){
  return _output_x;
}

int Arduino_Sprite::GetY(void){
  return _output_y;
}

void Arduino_Sprite::DrawFast(void){
  DrawFast(_output_x,_output_y);
}

void Arduino_Sprite::DrawFast(uint16_t sx, uint16_t sy){
  _output_x = sx;
  _output_y = sy;
  _output->draw16bitRGBBitmap(sx,sy,_framebuffer,_width,_height);
  return;

  // Not faster than the base function
  int i = 0;
  for(int ty=0; ty<_height;++ty){
    int iy = ((ty + sy) * _lineMod) + sx;
    for(int tx=0; tx<_width;++tx){
      _destBuffer[iy++] = _framebuffer[i++];
    }
  }


}


void Arduino_Sprite::DrawFastWithKey(void){
  DrawFastWithKey(_output_x, _output_y);
}



void Arduino_Sprite::DrawFastWithKey(uint16_t sx, uint16_t sy){

  //Save the X and y for the clear() method
  _output_x = sx;
  _output_y = sy;


  int h = _height;
  int w = _width;

  if( (sy + h) > _maxY){
    h -= ( (sy + h) - _maxY);
  }

  if( (sx + w) > _lineMod){
    w -= ( (sx + w) - _lineMod);
  }

 

  for(int y = 0; y < h;++y){

    int iy = (y * _width);
    int jy = ((y + sy) * _lineMod) + sx;

    for(int x = 0; x < w;++x){

      uint16_t p = _framebuffer[iy++];

      if(p != _key){
        _destBuffer[jy] = p;
      }
      jy +=1;
      

    }

  }

}

void Arduino_Sprite::SaveBackground(uint16_t sx, uint16_t sy){

  /*
  // The non fast methods act as fast ones, if no backing store allocated.
  if(!_destBuffer){
    return;
  }
  */

  int i = 0;

  for(int y=0; y<_height; ++y){

    int iy = ((sy+y) * _lineMod) + sx;

    for(int x=0; x < _width; ++x){
      _backingStore[i] = _destBuffer[ iy]; 

      i += 1;
      iy += 1;

    }

  }

}

void Arduino_Sprite::Draw(void){
  Draw(_output_x,_output_y);
}

void Arduino_Sprite::Draw(uint16_t sx, uint16_t sy){

  SaveBackground(sx,sy);
  DrawFast(sx,sy);

}

void Arduino_Sprite::DrawWithKey(void){
  DrawWithKey(_output_x,_output_y);
}

void Arduino_Sprite::DrawWithKey(uint16_t sx, uint16_t sy){

  SaveBackground(sx,sy);
  DrawFastWithKey(sx,sy);

}

void Arduino_Sprite::BlitFast(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h){
 
 if( (dx + w) > _lineMod){
    w -= ((dx+w) - _lineMod);

    if(w < 1){
      return;
    }

  }
 
  if( (dy + h) > _maxY){
    h -= ( (dy+h) - _maxY);

    if(h < 1){
      return;
    }

  }

  //int iy = ((dy) * _lineMod) + dx;

 

  for(int ty = 0; ty < h; ++ty){

    int iy = ((dy + ty) * _lineMod) + dx;
   // iy += (_lineMod - w);
    
    int jy = (((sy + ty) * _width)) + sx;

    for(int tx = 0; tx < w; ++tx){

          _destBuffer[ iy] = _framebuffer[ jy];

      iy += 1;
      jy += 1;

    }
  }
}


void Arduino_Sprite::BlitFastWithKey(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h){


  if( (dx + w) > _lineMod){

    w -= ((dx+w) - _lineMod);

    if(w < 1){
      return;
    }

  }
 
  if( (dy + h) > _maxY){

    h -= ( (dy+h) - _maxY);

      if(h < 1){
      return;
    }

  }



  for(int ty = 0; ty < h; ++ty){

    int iy = ((dy + ty) * _lineMod) + dx;
    int jy = (((sy + ty) * _width)) + sx;

    for(int tx = 0; tx < w; ++tx){

      
        uint16_t p = _framebuffer[ jy];
        jy += 1;

        if( p!= _key){
          _destBuffer[iy] = p;
        }

        iy += 1;
        
     

      
    }
  }




}



void Arduino_Sprite::BlitFastAligned(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h){
 
 if( (dx + w) > _lineMod){
    w -= ((dx+w) - _lineMod);

    if(w < 1){
      return;
    }

  }
 
  if( (dy + h) > _maxY){
    h -= ( (dy+h) - _maxY);

    if(h < 1){
      return;
    }

  }

  int r = w%4;
   w = w >> 1;



  for(int ty = 0; ty < h; ++ty){

    int iy = ((dy + ty) * _lineMod) + dx;
    int jy = (((sy + ty) * _width)) + sx;

    uint32_t* temp =(uint32_t*)&_framebuffer[jy];
    int ti = 0;

    for(int tx = 0; tx < w; ++tx){

      register uint32_t p = temp[ti++];
      register uint16_t q = p;

      _destBuffer[ iy] = q;
      iy += 1;
      jy += 1;

      q = p >> 16;
      _destBuffer[ iy] = q;
      iy += 1;
      jy += 1;

    }
  }
}



void Arduino_Sprite::Clear(void){
  if(_backingStore){
  _output->draw16bitRGBBitmap(_output_x,_output_y,_backingStore,_width,_height);
  }else{
   // _output->fillRect(_output_x,_output_y,_width,_height,_key);
  }
}