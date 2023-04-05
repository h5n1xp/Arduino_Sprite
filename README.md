# Arduino_Sprite
A Sprite/Blitter library for use with the wonderfully portable Arduino_GFX Library (though it should be possible to port this to other Graphics Libraries). 

This an Alpha version of my Sprite/Blitter library. The Sprite Class can take a 16bit (RGB 5:6:5) image or a 256 colour palette image, both require a 16bit framebuffer (or Arduino_GFX canvas). The Sprite Class can also use a 16bit canvas as an image source, so you can draw to it using the normal canvas draw methods.

Palette based sprites are faster and use less memory, but are limited to 256 colours per sprite (although each sprite can have its own unique 16bit palette, so this isn't too much of a limitation). 

This library is still in development and its API/types will be subject to frequent and breaking changes until a Beta is reached, and there will be many bugs not yet found.

Tested on the ESP32-S3 with a 320*170 ST7789 based TFT display (i.e. the LilyGo T3), an ESP32-S3 with a 3320*240 ST7789 TFT Display (i.e. the lilyGo T-HMI), and an ESP32-S3 with a 480*480 ST7701S based Round TFT display (i.e. the LilyGo T-RGB).

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

This is to specify the actual size of the image to use, as well out the output canvas which will usuall be the size of the display and will be uploaded during a flush() call. Currently the output canvas needs to be 16bit 5:6:6 RGB.

Before usage, one must call the begin() method which sets up the buffers and initilises all the sprite's properties. There are several methods to chose from.

sprite->begin(); // Will allocate an internal 16bit canvas object with the dimensions specified in the constructor

sprite->begin(canvas); // Will use an already allocated 16bit canvas.

sprite->begin(bitmap8Bit, palette); // Will take an allocated 8bit indexed bitmap and a palette (max 256 colours).

sprite->begin(bitmap16bit); // will take an allocated 16bit 5:6:5 RGB bitmap.

The sprites which are passed a bitmap do not have an associated canvas so can't accept drawing commands. If a you allow the sprite to allocate its own canvas, you can obtain a pointer with the GetCanvas() method, which can accept the normal Arduino_Canvas drawing methods.

If you need transparency, then use need to call the SetChromaKey(colour) method. If the sprite is canvas based or 16bit, then the colour value is the specific colour value to ignore during blitting. If the sprite is 8bit palette based then you need to specify the colour index (a value between 0 and 255). To use the chroma key, you need to call the "WithKey" methods, i.e. DrawFastWithKey(), otherwise the transparency will be ignored.


If the background of the display is static, then you need to call the SetBackingStore() method on the sprite and use the non-fast draw methods which save a copy of the background before drawing, which can then be restored by calling the Clear() method. This much faster if you only have a few small sprites moving over an unchanging background. Calling clear needs to be done in the reverse order to drawing.

If the background is drawn every frame (which is more common in modern games), then use the "fast" drawing methods, which don't save a copy of the background. There is no need to call a sprite's clear() method.

If the sprite is to be animated, then the bitmap/image the sprite is initilised with needs to be a sprite sheet (a single image with all frames of animation arranged in a regular grid). After initilisation, you need to call initAnim().

sprite->initAnim(totalFrameCount, firstFrameTopLeftX, firstFrameTopLeftY, frameWidth, frameHeight)

A special Scroll() method is also included which wraps an image around the display, useful for paralax scrolling.
