# Arduino_Sprite
A Sprite/Blitter library for use with the wonderfully portable Arduino_GFX Library (though it should be possible to port this to other Graphics Libraries). 

This an Alpha version of my Sprite/Blitter library. The Sprite Class can take a 16bit (RGB 5:6:5) image or a 256 colour palette image, both require a 16bit framebuffer (or Arduino_GFX canvas). The Sprite Class can also use a 16bit canvas as an image source, so you can draw to it using the normal canvas draw methods.

Palette based sprites are faster and use less memory, but are limited to 256 colours per sprite (although each sprite can have its own uniqut 16bit palette, so this isn't too much of a limitiation). 

This library is still in development and its API/types will be subject to frequent and breaking changes until a Beta is reached, and there will be many bugs not yet found.

Only tested on the ESP32-S3 with a 320*170 ST7789 based TFT display (i.e. the LilyGo T3).

Videos of this in action:
https://youtu.be/wZR8tQ5Uga4
https://youtu.be/56ELuOAAgyg

#Current Focus
1. Cleaning up the Draw methods to include animation frame and tilemap support. Also want to add playfields, to make smooth scrolling esiser.
2. Add RLE to the palette image format, this should reduce memory traffic and improve performance.
3. Release an image conversion application, to generate compatible data.

#Usage
The constrctor requires three parameters:

Arduino_Sprite* sprite = new Arduino_Sprite(width,height,output_canvas);

This is to specify the actual size of the image to use, as well out the output canvas which will usuall be the size of the display and will be uploaded during a flush() call. Currentlt the output canvas needs to be 16bit 5:6:6 RGB.

Before usage, one must call the begin() method which sets up the buffers and initilises all the sprite's properties. There are several methods to chose from.

sprite->begin(); // Will allocate an internal 16bit canvas object with the dimensions specified in the constructor
