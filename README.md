# Arduino_Sprite
A Sprite/Blitter library for use with the wonderfully portable Arduino_GFX Library (though it should be possible to port this to other Graphics Libraries). 

This an Alpha version of my Sprite/Blitter library. Currently only 2 classes are avilable, a 16bit (RGB 5:6:6) and 256 colour palette based on, both require a 16bit framebuffer (or Arduino_GFX canvas). Palette based sprites are faster and use less memory, but are limited to 256 colours per sprite (although each sprite can have its own palette, so this isn't too much of a limitiation). I have written a third, indexed based, class but at this time it is slower than using a 16bit framebuffer, so as no advantages over the palette based sprite.

This library is still in development and its API/types will be subject subject to frequent change until a Beta is reached, and there will be many bugs not yet found.

Only tested on the ESP32-S3 with a 320*170 ST7789 based TFT display (i.e. the LilyGo T3).
