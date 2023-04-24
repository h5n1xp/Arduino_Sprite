//
//  blitter.hpp
//  SpriteProjects
//
//  Created by Matt Parsons on 03/03/2023.
//

#ifndef blitter_h
#define blitter_h

#include <stdio.h>
#include <stdint.h>

class blitter_t{
public:
    virtual void BlitFast(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h);
    virtual void BlitFastWithKey(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h);
    
    virtual void BlitFastScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh);
    virtual void BlitFastWithKeyScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh);
    
    void Save(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t* buffer);
    void Restore(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t* buffer);
    
    
    uint8_t* _8bitBuffer;
    uint16_t* _16bitBuffer;
    uint16_t* _palette;
    uint16_t* _destBuffer;
    uint16_t _key;
    
    uint16_t _lineMod;
    uint16_t _maxY;
    uint16_t _width;
};


class blitter_palette_t : public blitter_t{
    void BlitFast(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h) override;
    void BlitFastWithKey(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h) override;
    
    void BlitFastScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh) override;
    void BlitFastWithKeyScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh) override;
};


class blitter_byteswap_t : public blitter_t{
    void BlitFast(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h) override;
    void BlitFastWithKey(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h) override;
    
    void BlitFastScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh) override;
    void BlitFastWithKeyScaled(int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t h, float sw, float sh) override;
    
};


#endif /* blitter_h */
