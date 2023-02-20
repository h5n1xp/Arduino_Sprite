# Arduino_Sprite
A Sprite/Blitter library for use with the wonderfully portable Arduino_GFX Library (though it should be possible to port this to other Graphics Libraries). 

This an Alpha version of my Sprite/Blitter library. Currently only 2 classes are available, a 16bit (RGB 5:6:5) sprite class and 256 colour palette based sprite class, both require a 16bit framebuffer (or Arduino_GFX canvas). 

Palette based sprites are faster and use less memory, but are limited to 256 colours per sprite (although each sprite can have its own uniqut 16bit palette, so this isn't too much of a limitiation). 

I have written a third, indexed based, class but at this time it is slower than using a 16bit framebuffer, so has no advantages over the palette based sprite. This is not currently included.

This library is still in development and its API/types will be subject to frequent and breaking changes until a Beta is reached, and there will be many bugs not yet found.

Only tested on the ESP32-S3 with a 320*170 ST7789 based TFT display (i.e. the LilyGo T3).

Videos of this in action:
https://youtu.be/wZR8tQ5Uga4
https://youtu.be/56ELuOAAgyg

#Current Focus
1. Cleaning up the Draw methods to include animation frame and tilemap support. Also want to add playfields, to make smooth scrolling esiser.
2. Add RLE to the palette image format, this should reduce memory traffic and improve performance.
3. Release an image conversion application, to generate compatible data.

