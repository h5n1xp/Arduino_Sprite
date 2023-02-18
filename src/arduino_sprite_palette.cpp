//
//  arduino_sprite_palette.cpp
//  Arduino_Sprite
//
//  Created by Matt Parsons on 27/01/2023.
//  Copyright © 2023 Matt Parsons. All rights reserved.
//

#include "arduino_sprite_palette.h"

typedef union {
    uint32_t value;
    struct{
     uint8_t lsb;
     uint8_t mlb;
     uint8_t mub;
     uint8_t msb;
    };
}dat32_t;


Arduino_Sprite_Palette::Arduino_Sprite_Palette(int16_t w, int16_t h, Arduino_GFX *output): Arduino_Canvas(w, h, output){

}


void Arduino_Sprite_Palette::begin(void){
 // Since there is no framebuffer of the Arduino_G type, and sprites won't really work on them anyway, 
  // cast to an Arduino_Canvas type to use the getFramebuffer() method.
  Arduino_Canvas* c = (Arduino_Canvas*)_output; 
  _destBuffer = c->getFramebuffer();
  _lineMod = c->width();
  _maxY = c->height();
  _backingStore = NULL;
  _framebuffer = NULL;
  _buffer = NULL;


    size_t s = _width * _height;
#if defined(ESP32)
    if (psramFound())
    {
        _buffer = (uint8_t *)ps_malloc(s);
    }
    else
    {
        _buffer = (uint8_t *)malloc(s);
        if (!_buffer)
        {
            // hack for allocate memory over 63,360 pixels
            s /= 2;
            _buffer = (uint8_t *)malloc(s);
            uint8_t *tmp = (uint8_t *)malloc(s);
            UNUSED(tmp);
            log_v("_framebuffer delta: %d", tmp - _buffer);
        }
    }
#else
    _buffer = (uint8_t *)malloc(s);
#endif
    if (!_buffer)
    {
        Serial.println(F("_buffer allocation failed."));
    }
  
}


void Arduino_Sprite_Palette::begin(uint8_t* source, uint16_t* pal){
    Arduino_Canvas* c = (Arduino_Canvas*)_output; 
  _destBuffer = c->getFramebuffer();
  _lineMod = c->width();
  _maxY = c->height();
  _framebuffer = NULL;
  _backingStore = NULL;
  _buffer = source;
  _palette = pal;
}

void Arduino_Sprite_Palette::begin(int32_t speed){
  //NOP
  //This method has no meaning for sprites
  begin();
}

void Arduino_Sprite_Palette::SetBackingStore(void){

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

void Arduino_Sprite_Palette::SetChromaKey(uint8_t chromaKey){
  _key = chromaKey;

  if(_backingStore){
    int i = 0;
    for(int y=0;y<_height;++y){
      for(int x=0;x<_width;++x){

          _backingStore[i++] = _palette[_key];
    
      }
    }
  }
}

void Arduino_Sprite_Palette::Move(int sx, int sy){
  _output_x = sx;
  _output_y = sy;
}

int Arduino_Sprite_Palette::GetX(void){
  return _output_x;
}

int Arduino_Sprite_Palette::GetY(void){
  return _output_y;
}

void Arduino_Sprite_Palette::DrawFast(void){
  DrawFast(_output_x,_output_y);
}

void Arduino_Sprite_Palette::DrawFast(uint16_t sx, uint16_t sy){
  _output_x = sx;
  _output_y = sy;
 
  // Much faster than the base drawIndexedBitmap function
 int w = _width;
 int h = _height;
 if( (sx + w) > _lineMod){
    w -= ((sx+w) - _lineMod);

    if(w < 1){
      return;
    }

  }
 
  if( (sy + h) > _maxY){
    h -= ( (sy+h) - _maxY);

    if(h < 1){
      return;
    }

  }

  //int i = 0;
  for(int ty=0; ty<h;++ty){
    int i = (ty * _width);
    int iy = ((ty + sy) * _lineMod) + sx;
    for(int tx=0; tx<w;++tx){
      _destBuffer[iy] = _palette[_buffer[i]];

      iy += 1;
      i += 1;

    }
  }


}


void Arduino_Sprite_Palette::DrawFastWithKey(void){
  DrawFastWithKey(_output_x, _output_y);
}



void Arduino_Sprite_Palette::DrawFastWithKey(uint16_t sx, uint16_t sy){

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

      uint8_t p = _buffer[iy++];

      if(p != _key){
        _destBuffer[jy] = _palette[p];
      }
      jy +=1;
      

    }

  }

}

void Arduino_Sprite_Palette::SaveBackground(uint16_t sx, uint16_t sy){

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

void Arduino_Sprite_Palette::Draw(void){
  Draw(_output_x,_output_y);
}

void Arduino_Sprite_Palette::Draw(uint16_t sx, uint16_t sy){

  SaveBackground(sx,sy);
  DrawFast(sx,sy);

}

void Arduino_Sprite_Palette::DrawWithKey(void){
  DrawWithKey(_output_x,_output_y);
}

void Arduino_Sprite_Palette::DrawWithKey(uint16_t sx, uint16_t sy){

  SaveBackground(sx,sy);
  DrawFastWithKey(sx,sy);

}

void Arduino_Sprite_Palette::BlitFastOLD(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h){
 
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

          _destBuffer[ iy] = _palette[_buffer[ jy]];

      iy += 1;
      jy += 1;

    }
  }
}


void Arduino_Sprite_Palette::BlitFastWithKeyOLD(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h){


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

      
        uint8_t p = _buffer[ jy];
        jy += 1;

        if( p!= _key){
          _destBuffer[iy] = _palette[p];
        }

        iy += 1;
        
     

      
    }
  }

}


void Arduino_Sprite_Palette::BlitFastWithKey(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h){

  //sx = sx & 0xFFFC; //32bit alignment

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

  w = w  >> 2;
 



  for(int ty = 0; ty < h; ++ty){

    register int iy = ((dy + ty) * _lineMod) + dx;
    register int jy = (((sy + ty) * _width)) + sx;

    register uint32_t* temp =(uint32_t*)&_buffer[jy];
    register int ti = 0;

    for(int tx = 0; tx < w; ++tx){

      register uint32_t in = temp[ti++];
      register uint8_t p = in ;

      if( p!= _key){
        _destBuffer[iy] = _palette[p];
      }
      iy += 1;
        
     
      p = (in >> 8) ;
      if( p!= _key){
        _destBuffer[iy] = _palette[p];
      }
      iy += 1;
        

      p = (in >> 16) ;
      if( p!= _key){
        _destBuffer[iy] = _palette[p];
      }
      iy += 1;

      p = in >> 24;
      if( p!= _key){
        _destBuffer[iy] = _palette[p];
      }
      iy += 1;

    }

    if(r>0){

      register uint32_t in = temp[ti++];

      register uint8_t p = in ;

      if( p!= _key){
        _destBuffer[iy] = _palette[p];
      }
      iy += 1;
        
        
      if(r>1){
        p = (in >> 8) ;
        
        if( p!= _key){
          _destBuffer[iy] = _palette[p];
        }
        iy += 1;
        
        if(r>2){
          p = (in >> 16) ;

          if( p!= _key){
            _destBuffer[iy] = _palette[p];
          }
          iy += 1;
    
        }
      }
      
    }

  }
  

}


void Arduino_Sprite_Palette::Clear(void){
  if(_backingStore){
  _output->draw16bitRGBBitmap(_output_x,_output_y,_backingStore,_width,_height);
  }else{
   // _output->fillRect(_output_x,_output_y,_width,_height,_key);
  }
}







void Arduino_Sprite_Palette::BlitFast(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, int16_t w, int16_t h){

 // sx = sx & 0xFFFC; //32bit alignment

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
  w = w / 4;
 

  for(int ty = 0; ty < h; ++ty){

    register int iy = ((dy + ty) * _lineMod) + dx;
    register int jy = (((sy + ty) * _width)) + sx;

    register uint32_t* temp =(uint32_t*)&_buffer[jy];
    register int ti = 0;

    register uint32_t* out =(uint32_t*)&_destBuffer[iy];
    register int ot = 0;

    for(int tx = 0; tx < w; ++tx){
      
      //Read in 4 8bit pixels
      register uint32_t in = temp[ti++];
      
      //Write out first 2 16bit pixels
      register uint8_t p = in;
      register uint32_t tp = _palette[p];

      p = (in >> 8);
      tp = (_palette[p] << 16) | tp;

      out[ot++] =  tp;

      //Next two 16bit pixels
      p = (in >> 16);
      register uint32_t tp2 = _palette[p];

      p = in >> 24;
      tp2 = (_palette[p] << 16) | tp2;

      out[ot++] = tp2;
      iy +=4;


    }

    if(r>0){

      register uint32_t in = temp[ti++];
      register uint8_t p = in ;

      _destBuffer[iy] = _palette[p];
      iy += 1;
        
        
      if(r>1){
        p = (in >> 8) ;
        _destBuffer[iy] = _palette[p];
        iy += 1;
        
        if(r>2){
          p = (in >> 16) ;
          _destBuffer[iy] = _palette[p];
          iy += 1;
        }
      }
      
    }


  }

}