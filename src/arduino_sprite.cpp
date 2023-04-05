//
//  arduino_sprite.cpp
//  SpriteProjects
//
//  Created by Matt Parsons on 06/03/2023.
//

#include "arduino_sprite.h"

Arduino_Sprite::Arduino_Sprite(int16_t w, int16_t h, Arduino_GFX* output){
    _output = output;
    _frame = 0;
    _fw = w;
    _fh = h;
    _fx = 0;
    _fy = 0;
    _backingStore = NULL;
    _canvas = NULL;
}

Arduino_Canvas*  Arduino_Sprite::begin(){
    _canvas = new Arduino_Canvas(_fw,_fh,_output);
    _canvas->begin(GFX_SKIP_OUTPUT_BEGIN);
    begin(_canvas->getFramebuffer());
    return _canvas;
}

void Arduino_Sprite::begin(Arduino_Canvas* canvas){
    _canvas = canvas;
    begin(_canvas->getFramebuffer());
}

void Arduino_Sprite::begin(uint8_t* source, uint16_t* pal){
    Arduino_Canvas* c = (Arduino_Canvas*)_output;
    _blitter = new blitter_palette_t();
    
  _blitter->_destBuffer = c->getFramebuffer();
  _blitter->_lineMod = c->width();
  _blitter->_maxY = c->height();
  _blitter->_width = _fw;
  _blitter->_8bitBuffer = source;
  _blitter->_16bitBuffer = NULL;
  _blitter->_palette = pal;
}

void Arduino_Sprite::begin(uint16_t* source){
    Arduino_Canvas* c = (Arduino_Canvas*)_output;
    _blitter = new blitter_t();
    
  _blitter->_destBuffer = c->getFramebuffer();
  _blitter->_lineMod = c->width();
  _blitter->_maxY = c->height();
  _blitter->_width = _fw;
  _blitter->_8bitBuffer = NULL;
  _blitter->_16bitBuffer = source;
  _blitter->_palette = NULL;
}

void Arduino_Sprite::SetBackingStore(void){

    size_t s = _fw * _fh * 2;

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
  _blitter->_key = chromaKey;

  if(_backingStore){
    int i = 0;
    for(int y=0;y<_fh;++y){
      for(int x=0;x<_fw;++x){

          _backingStore[i++] = _blitter->_palette[_blitter->_key];
    
      }
    }
  }
}


void Arduino_Sprite::initAnim(uint16_t count, int x, int y, int w, int h){
    _frames = count;
    _fx = x;
    _fy = y;
    _fw = w;
    _fh = h;
}


void Arduino_Sprite::Frame(uint16_t frame){
    _frame = frame;
}

void Arduino_Sprite::Move(int16_t sx, int16_t sy){
  _output_x = sx;
  _output_y = sy;
}

void Arduino_Sprite::Move(int16_t sx, int16_t sy,uint16_t frame){
    _frame = frame;
  _output_x = sx;
  _output_y = sy;
}



int Arduino_Sprite::GetX(void){
    return _output_x;
}

int Arduino_Sprite::GetY(void){
    return _output_y;
}

int Arduino_Sprite::GetFrame(void){
    return _frame;
}

int Arduino_Sprite::GetFrames(void){
    return _frames;
}

Arduino_Canvas* Arduino_Sprite::GetCanvas(void){
    return _canvas;
}

void Arduino_Sprite::DrawFastWithKey(void){
  DrawFastWithKey(_output_x, _output_y, _frame);
}

void Arduino_Sprite::DrawFastWithKey(uint16_t frame){
    DrawFastWithKey(_output_x, _output_y, frame);
}

void Arduino_Sprite::DrawFastWithKey(int16_t x, int16_t y){
    DrawFastWithKey(x, y, _frame);
}

void Arduino_Sprite::DrawFastWithKey(int16_t x, int16_t y, uint16_t frame){
    _blitter->BlitFastWithKey(x, y, _fx + (_fw * frame), _fy, _fw, _fh);
}

void Arduino_Sprite::ScrollFastWithKey(){
    
    if(_output_x<0){
        DrawFastWithKey(_output_x, _output_y, _frame);
        DrawFastWithKey(_fw + _output_x, _output_y, _frame);
    }else{
        DrawFastWithKey(_output_x, _output_y, _frame);
        DrawFastWithKey(_output_x -_fw, _output_y, _frame);
    }
}

void Arduino_Sprite::DrawFast(void){
  DrawFast(_output_x, _output_y, _frame);
}

void Arduino_Sprite::DrawFast(uint16_t frame){
    DrawFast(_output_x, _output_y, frame);
}

void Arduino_Sprite::DrawFast(int16_t x, int16_t y){
    DrawFast(x, y, _frame);
}

void Arduino_Sprite::DrawFast(int16_t x, int16_t y, uint16_t frame){
    int tx = _fx + (_fw * frame);
    _blitter->BlitFast(x, y, tx, _fy, _fw, _fh);
}


void Arduino_Sprite::ScrollFast(){
    
    if(_output_x<0){
        DrawFast(_output_x, _output_y, _frame);
        DrawFast(_fw + _output_x, _output_y, _frame);
    }else{
        DrawFast(_output_x, _output_y, _frame);
        DrawFast(_output_x -_fw, _output_y, _frame);
    }
}




void Arduino_Sprite::Draw(){
    SaveBackground(_output_x, _output_y);
    DrawFast();
}

void Arduino_Sprite::Draw(uint16_t frame){
    SaveBackground(_output_x, _output_y);
    DrawFast(frame);
}

void Arduino_Sprite::Draw(int16_t x, int16_t y){
    SaveBackground(x, y);
    DrawFast(x, y, _frame);
}

void Arduino_Sprite::Draw(int16_t x, int16_t y, uint16_t frame){
    SaveBackground(x, y);
    DrawFast(x, y, frame);
}



void Arduino_Sprite::DrawWithKey(){
    SaveBackground(_output_x, _output_y);
    DrawFastWithKey();
}

void Arduino_Sprite::DrawWithKey(uint16_t frame){
    SaveBackground(_output_x, _output_y);
    DrawFast(frame);
}

void Arduino_Sprite::DrawWithKey(int16_t x, int16_t y){
    SaveBackground(x, y);
    DrawFastWithKey(x, y, _frame);
}

void Arduino_Sprite::DrawWithKey(int16_t x, int16_t y, uint16_t frame){
    SaveBackground(x, y);
    DrawFastWithKey(x, y, frame);
}




void Arduino_Sprite::SaveBackground(int16_t x, int16_t y){
    if(_backingStore){
        _blitter->Save(x,y, _fw, _fh, _backingStore);
    }
}


void Arduino_Sprite::Clear(){
    if(_backingStore){
        _blitter->Restore(_output_x,_output_y, _fw, _fh, _backingStore);
    }
}
