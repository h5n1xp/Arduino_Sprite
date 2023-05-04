//
//  blitter.cpp
//  SpriteProjects
//
//  Created by Matt Parsons on 03/03/2023.
//

#include "blitter.h"

typedef union {
    uint32_t value;
    struct{
     uint8_t lsb;
     uint8_t mlb;
     uint8_t mub;
     uint8_t msb;
    };
}dat32_t;


void blitter_t::BlitFast(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h){
    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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

      uint16_t* temp =&_16bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      for(int tx = 0; tx < w; ++tx){

        //Byte swap for dave
        //uint16_t p = temp[tx];
        //p = p << 8 | p >> 8;
        //out[tx] = p;

        out[tx] = temp[tx];
      }


    }

    
}
void blitter_t::BlitFastWithKey(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h){
    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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

      uint16_t* temp =&_16bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      for(int tx = 0; tx < w; ++tx){
          
          uint16_t pixel = temp[tx];
          
          if(pixel != _key){
            //byte swapping for dave
            //pixel = pixel << 8 | pixel >> 8;
            out[tx] = pixel;
          }
      }


    }

    
}

void blitter_t::BlitFastScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh){
    
    int frame = sx;
    sx = 0;
    
    int iw = w;
    int ih = h;
    
    w *=sw;
    h *= sh;
    
    
    
    
    float wi = 0;
    if(w != 0){
        wi = (float)(iw) / (float)(w);
    }
    
    
    float hi = 0;
    if(h != 0){
        hi = (float)(ih) / (float)h;
    }
    
    
    
    if(dx < 0){
        sx -= dx;
        w +=dx;
        dx = 0;
    }
    
    if(dy < 0){
        sy -= dy;
        h +=dy;
        dy = 0;
    }
    
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
    
    
    
    
    float tempy = sy * hi;
    
    
    
    
    for(int ty = 0; ty < h; ++ty){
        
        int iy = ((dy + ty) * _lineMod) + dx;
        int jy = ( (int)(tempy) * _width) + frame;
        tempy += hi;
        
        uint16_t* temp =&_16bitBuffer[jy];
        uint16_t* out =&_destBuffer[iy];
        
        float tempx = sx * wi;
        
        for(int tx = 0; tx < w; ++tx){
            
            uint16_t pixel = temp[(int)tempx];
            tempx += wi;
            
            //byte swapping for dave
            //pixel = pixel << 8 | pixel >> 8;
            
            out[tx] = pixel;
            
        }
        
        
    }
    
}
void blitter_t::BlitFastWithKeyScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh){

    int frame = sx;
    sx = 0;
    int iw = w;
    int ih = h;
    
    w *=sw;
    h *= sh;

    
    
    
    float wi = 0;
    if(w != 0){
        wi = (float)(iw) / (float)(w);
    }
    
    
    float hi = 0;
    if(h != 0){
        hi = (float)(ih) / (float)h;
    }
    
    

    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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



    


    float tempy = sy * hi;

    
    for(int ty = 0; ty < h; ++ty){

      int iy = ((dy + ty) * _lineMod) + dx;
      int jy = (((int)(tempy) * _width) + frame); //int jy = (((int)(tempy) * _width));
      tempy += hi;
        
      uint16_t* temp =&_16bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      float tempx = sx * wi;

      for(int tx = 0; tx < w; ++tx){
          
          uint16_t pixel = temp[(int)tempx];
          tempx += wi;
          
          if(pixel != _key){
            //byte swapping for dave
            //pixel = pixel << 8 | pixel >> 8;

          out[tx] = pixel;
        }
      }


    }
    
}

void blitter_t::Save(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t* buffer){
    

    if(x < 0){
        w += x;
        x = 0;
    }

    if(y < 0){
        h +=y;
        y = 0;
    }
    
    
    if( (x + w) > _lineMod){
        
        w -= (x + w) - _lineMod;
     
        if(w < 1){
            return;
        }
        
    }
           
    if( (y + h) > _maxY){
        
        h -= (y + h) - _maxY;
        
        if(h < 1){
            return;
        }
     
    }
         
    
    int i = 0;
    
    //printf("%d, %d, %d, %d\n",x,y,w,h);
    
    
    
    for(int ty=0; ty<h; ++ty){
        
        uint16_t* temp = &_destBuffer[ (ty+y)*_lineMod + x];
        
        for(int tx=0; tx<w; ++tx){
            
            buffer[i++] = temp[tx];
            //temp[tx] = 0xFFFF;
        }
    }
    
    return;
}
void blitter_t::Restore(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t* buffer){

    if(x < 0){
        w += x;
        x = 0;
    }

    if(y < 0){
        h +=y;
        y = 0;
    }
    
    
    if( (x + w) > _lineMod){
        
        w -= (x + w) - _lineMod;
        
        if(w < 1){
            return;
        }
        
    }
           
    if( (y + h) > _maxY){
        
        h -= (y + h) - _maxY;
        
        if(h < 1){
            return;
        }
        
    }
             
    int i = 0;
    
    for(int ty=0; ty<h; ++ty){
        
        uint16_t* temp = &_destBuffer[ (ty+y)*_lineMod + x];
        
        for(int tx=0; tx<w; ++tx){
            
            temp[tx] = buffer[i++];
            
        }
    }
    
}

//********************************************* Palette

void blitter_palette_t::BlitFastWithKey(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h){

  //sx = sx & 0xFFFC; //32bit alignment

  if(dx < 0){
    sx -= dx;
    w +=dx;
    dx = 0;
  }

  if(dy < 0){
    sy -= dy;
    h +=dy;
    dy = 0;
  }

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

    int iy = ((dy + ty) * _lineMod) + dx;
    int jy = (((sy + ty) * _width)) + sx;

    uint32_t* temp =(uint32_t*)&_8bitBuffer[jy];
    int ti = 0;

    for(int tx = 0; tx < w; ++tx){

      uint32_t in = temp[ti++];
      uint8_t p = in ;

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

      uint32_t in = temp[ti++];

      uint8_t p = in ;

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
void blitter_palette_t::BlitFast(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h){

 // sx = sx & 0xFFFC; //32bit alignment

  if(dx < 0){
    sx -= dx;
    w +=dx;
    dx = 0;
  }

  if(dy < 0){
    sy -= dy;
    h +=dy;
    dy = 0;
  }

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

    int iy = ((dy + ty) * _lineMod) + dx;
    int jy = (((sy + ty) * _width)) + sx;

    uint32_t* temp =(uint32_t*)&_8bitBuffer[jy];
    int ti = 0;

    uint32_t* out =(uint32_t*)&_destBuffer[iy];
    int ot = 0;

    for(int tx = 0; tx < w; ++tx){
      
      //Read in 4 8bit pixels
      uint32_t in = temp[ti++];
      
      //Write out first 2 16bit pixels
      uint8_t p = in;
      uint32_t tp = _palette[p];

      p = (in >> 8);
      tp = (_palette[p] << 16) | tp;

      out[ot++] =  tp;

      //Next two 16bit pixels
      p = (in >> 16);
      uint32_t tp2 = _palette[p];

      p = in >> 24;
      tp2 = (_palette[p] << 16) | tp2;

      out[ot++] = tp2;
      iy +=4;


    }

    if(r>0){

      uint32_t in = temp[ti++];
      uint8_t p = in ;

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

void blitter_palette_t::BlitFastScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh){
    
    int frame = sx;
    sx = 0;
    
    int iw = w;
    int ih = h;
    
    w *=sw;
    h *= sh;
    
    
    
    
    float wi = 0;
    if(w != 0){
        wi = (float)(iw) / (float)(w);
    }
    
    
    float hi = 0;
    if(h != 0){
        hi = (float)(ih) / (float)h;
    }
    
    
    
    if(dx < 0){
        sx -= dx;
        w +=dx;
        dx = 0;
    }
    
    if(dy < 0){
        sy -= dy;
        h +=dy;
        dy = 0;
    }
    
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
    
    
    
    
    float tempy = sy * hi;
    
    
    
    
    for(int ty = 0; ty < h; ++ty){
        
        int iy = ((dy + ty) * _lineMod) + dx;
        int jy = ( (int)(tempy) * _width) + frame;
        tempy += hi;
        
        uint8_t* temp =&_8bitBuffer[jy];
        uint16_t* out =&_destBuffer[iy];
        
        float tempx = sx * wi;
        
        for(int tx = 0; tx < w; ++tx){
            
            uint8_t colour = temp[(int)tempx];
            tempx += wi;
            
            //byte swapping for dave
            //pixel = pixel << 8 | pixel >> 8;
            
            out[tx] = _palette[colour];
            
        }
        
        
    }
    
}

void blitter_palette_t::BlitFastWithKeyScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh){

    int frame = sx;
    sx = 0;
    int iw = w;
    int ih = h;
    
    w *=sw;
    h *= sh;

    
    
    
    float wi = 0;
    if(w != 0){
        wi = (float)(iw) / (float)(w);
    }
    
    
    float hi = 0;
    if(h != 0){
        hi = (float)(ih) / (float)h;
    }
    
    

    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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



    


    float tempy = sy * hi;

    
    for(int ty = 0; ty < h; ++ty){

      int iy = ((dy + ty) * _lineMod) + dx;
      int jy = (((int)(tempy) * _width) + frame); //int jy = (((int)(tempy) * _width));
      tempy += hi;
        
      uint8_t* temp =&_8bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      float tempx = sx * wi;

      for(int tx = 0; tx < w; ++tx){
          
          uint8_t colour = temp[(int)tempx];
          tempx += wi;
          
          if(colour != _key){
            //byte swapping for dave
            //pixel = pixel << 8 | pixel >> 8;

              out[tx] = _palette[colour];
        }
      }


    }
    
}

// Byte Swap ************************************************************

void blitter_byteswap_t::BlitFast(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h){
    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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

      uint16_t* temp =&_16bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      for(int tx = 0; tx < w; ++tx){

        uint16_t p = temp[tx];
        p = p << 8 | p >> 8;
        out[tx] = p;

        //out[tx] = temp[tx];
      }


    }

    
}
void blitter_byteswap_t::BlitFastWithKey(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h){
    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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

      uint16_t* temp =&_16bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      for(int tx = 0; tx < w; ++tx){
          
          uint16_t pixel = temp[tx];
          
          if(pixel != _key){
            pixel = pixel << 8 | pixel >> 8;
            out[tx] = pixel;
          }
      }


    }

    
}

void blitter_byteswap_t::BlitFastScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh){
    
    int frame = sx;
    sx = 0;
    
    int iw = w;
    int ih = h;
    
    w *=sw;
    h *= sh;

    
    
    
    float wi = 0;
    if(w != 0){
        wi = (float)(iw) / (float)(w);
    }
    
    
    float hi = 0;
    if(h != 0){
        hi = (float)(ih) / (float)h;
    }
    
    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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



    



    float tempy = sy * hi;
    
    for(int ty = 0; ty < h; ++ty){

      int iy = ((dy + ty) * _lineMod) + dx;
      int jy = ((int)(tempy) * _width) + frame;
      tempy += hi;
        
      uint16_t* temp =&_16bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      float tempx = sx * wi;

      for(int tx = 0; tx < w; ++tx){
          
          uint16_t pixel = temp[(int)tempx];
          tempx += wi;
          
            //byte swapping for dave
            pixel = pixel << 8 | pixel >> 8;

          out[tx] = pixel;

      }


    }



    
}

void blitter_byteswap_t::BlitFastWithKeyScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh){
    
    int frame = sx;
    sx = 0;
    
    int iw = w;
    int ih = h;
    
    w *=sw;
    h *= sh;

    
    
    
    float wi = 0;
    if(w != 0){
        wi = (float)(iw) / (float)(w);
    }
    
    
    float hi = 0;
    if(h != 0){
        hi = (float)(ih) / (float)h;
    }
    
    
    if(dx < 0){
      sx -= dx;
      w +=dx;
      dx = 0;
    }

    if(dy < 0){
      sy -= dy;
      h +=dy;
      dy = 0;
    }

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



    



    float tempy = sy * hi;
    
    for(int ty = 0; ty < h; ++ty){

      int iy = ((dy + ty) * _lineMod) + dx;
      int jy = ((int)(tempy) * _width) + frame;
      tempy += hi;
        
      uint16_t* temp =&_16bitBuffer[jy];
      uint16_t* out =&_destBuffer[iy];

      float tempx = sx * wi;

      for(int tx = 0; tx < w; ++tx){
          
          uint16_t pixel = temp[(int)tempx];
          tempx += wi;
          
          if(pixel != _key){
            //byte swapping for dave
            pixel = pixel << 8 | pixel >> 8;

          out[tx] = pixel;
        }
      }


    }

    
}
