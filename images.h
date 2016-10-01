/*
 * Exported with brandontools v1.0
 * Invocation command was brandontools -mode4 images back.png front.png sideL.png sideR.png Splash.png 
 * Time-stamp: Monday 11/10/2014, 01:50:59
 * 
 * Image Information
 * -----------------
 * back.png 8@16
 * front.png 8@16
 * sideL.png 9@16
 * sideR.png 9@16
 * Splash.png 240@160
 * 
 * Quote/Fortune of the Day!
 * -------------------------
 * Why do we want intelligent terminals when there are so many stupid users?
 * 
 * Education is the ability to meet life's situations.
 * 
 * All bug reports / feature requests are to be sent to Brandon (brandon.whitehead@gatech.edu)
 */

#ifndef IMAGES_BITMAP_H
#define IMAGES_BITMAP_H

extern const unsigned short images_palette[256];
#define IMAGES_PALETTE_SIZE 256

extern const unsigned short back[64];
#define BACK_SIZE 64
#define BACK_WIDTH 8
#define BACK_HEIGHT 16

extern const unsigned short front[64];
#define FRONT_SIZE 64
#define FRONT_WIDTH 8
#define FRONT_HEIGHT 16

extern const unsigned short sideL[72];
#define SIDEL_SIZE 72
#define SIDEL_WIDTH 9
#define SIDEL_HEIGHT 16

extern const unsigned short sideR[72];
#define SIDER_SIZE 72
#define SIDER_WIDTH 9
#define SIDER_HEIGHT 16

extern const unsigned short Splash[19200];
#define SPLASH_SIZE 19200
#define SPLASH_WIDTH 240
#define SPLASH_HEIGHT 160

#endif

