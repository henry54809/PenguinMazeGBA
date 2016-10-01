#include "mylib.h"
#include <math.h>
u16 *videoBuffer = (u16*) 0x6000000;
u16 *backBuffer =  (u16*) 0x600A000;


//Functions
// A function to set pixel (r, c) to the color passed in
void setPixel3(int r, int c, u16 color){
    videoBuffer[240*r + c] = color;
}

void setPixel4(int r, int c, u8 clrid)
{
    u16 *dst= &backBuffer[(r*240+c)/2];  // Division by 2 due to u8/u16 pointer mismatch!
    if(c&1)
        *dst= (*dst& 0xFF) | (clrid<<8);    // odd pixel
    else
        *dst= (*dst&~0xFF) |  clrid;        // even pixel
}

void drawRect(int r, int c, int width, int height, u16 color){
    int i;
    for(i=0; i<height; i++)
    {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[(r+i)*240+c];
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
    }
}

void drawHollowRect3(int r, int c, int width, int height, u16 color){ 
    int i; 
    for ( i=c; i< ( c+ width); i++ ){ 
       if ( i < 240 && r < 160 && i > 0 && r > 0 ){ 
            setPixel3( r, i, color );   
       } 
 
       if ( (r + height) < 160 && i < 240 && i > 0 && ( r+ height ) > 0 ){ 
            setPixel3( r + height - 1, i, color ); 
       } 
    } 
 
    for ( i=r; i< ( r+ height ); i++ ){ 
       if ( i < 160 && c < 240 && i > 0 && c > 0 ){ 
            setPixel3( i, c, color );   
       } 
 
       if ( (c + width) < 240 && i < 160 && i > 0 && ( c + width ) > 0 ){ 
            setPixel3( i, c + width - 1, color ); 
       } 
    } 
 
} 

void drawHollowRect4(int r, int c, int width, int height, u8 clrid){
    int i; 
    for ( i=c; i< ( c+ width); i++ ){ 
       if ( i < 240 && r < 160 && i > 0 && r > 0 ){ 
            setPixel4( r, i, clrid );   
       } 
 
       if ( (r + height) < 160 && i < 240 && i > 0 && ( r+ height ) > 0 ){ 
            setPixel4( r + height - 1, i, clrid ); 
       } 
    } 
 
    for ( i=r; i< ( r+ height ); i++ ){ 
       if ( i < 160 && c < 240 && i > 0 && c > 0 ){ 
            setPixel4( i, c, clrid );   
       } 
 
       if ( (c + width) < 240 && i < 160 && i > 0 && ( c + width ) > 0 ){ 
            setPixel4( i, c + width - 1, clrid ); 
       } 
    } 
 
}

void plotLine3(int x0, int y0, int x1, int y1, u16 color){ 
 
    int dx = fabs( x1 - x0 );
    int signX = x0 < x1 ? 1 : -1;
    int dy = fabs( y1 - y0 );
    int signY = y0 < y1 ? 1 : -1; 
    int err = ( (dx > dy ? dx : -dy) >>1 );

    while(1){
        
        setPixel3(x0,y0, color);
        
        if (x0==x1 && y0 == y1) 
            break;
        int e2 = err;
        
        if (e2 >-dx) { 
            err -= dy; 
            x0 += signX; 
        }
        
        if (e2 < dy) { 
            err += dx; 
            y0 += signY; 
        }
    }
}

void plotLine4(int x0, int y0, int x1, int y1, u8 clrid){ 
 
    int dx = fabs( x1 - x0 );
    int signX = x0 < x1 ? 1 : -1;
    int dy = fabs( y1 - y0 );
    int signY = y0 < y1 ? 1 : -1; 
    int err = ( (dx > dy ? dx : -dy) >>1 );

    while(1){
        
        setPixel4(x0,y0, clrid);
        
        if (x0==x1 && y0 == y1) 
            break;
        int e2 = err;
        
        if (e2 >-dx) { 
            err -= dy; 
            x0 += signX; 
        }
        
        if (e2 < dy) { 
            err += dx; 
            y0 += signY; 
        }
    }
}


void waitForVBlank()
{
    while(SCANLINECOUNTER>160);
    while(SCANLINECOUNTER<160);
}

void fillScreen(volatile unsigned char color)
{
    volatile unsigned short c = color << 8 | color; 
    DMA[3].src = &c;
    DMA[3].dst = backBuffer;
    DMA[3].cnt = 19200 | DMA_ON | DMA_SOURCE_FIXED; 
}

void drawImage3(int r, int c, int width, int height, const u16* image){
    int i;
    for ( i = 0; i < height; i++){
        DMA[3].src = &image[i*width];
        DMA[3].dst = &videoBuffer[(r+i)*240+c];
        DMA[3].cnt = width | DMA_ON; 
    }
}

void drawImage4(int r, int c, int width, int height, const u16* image) {
    int i;
    for ( i = 0; i < height; i++){
        DMA[3].src = &image[i*width];
        DMA[3].dst = &videoBuffer[(r+i)*240+c];
        DMA[3].cnt = width | DMA_ON; 
    }
}

void fillPortion(int r, int c, int width, int height, u8 color ){
    volatile unsigned short color16 = color << 8 | color;     
    int i;
    for ( i = 0; i < height; i++){
        DMA[3].src = &color16;
        DMA[3].dst = &backBuffer[((r+i)*240/2+c)];
        DMA[3].cnt = width/2 | DMA_ON | DMA_SOURCE_FIXED; 
    }
}

u16* PageFlip()
{
    u16 * temp = backBuffer;
    backBuffer = videoBuffer;
    videoBuffer = temp;
    REG_DISPCNT = REG_DISPCNT ^ DCNT_PAGE;            // update control register
    return backBuffer;
}
