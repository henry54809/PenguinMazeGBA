//Type definitions
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;
#define SCANLINECOUNTER *(volatile unsigned short*) 0x4000006

/* DMA */

#define REG_DMA0SAD         *(const volatile u32*)0x40000B0 // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(const volatile u32*)0x40000BC // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(const volatile u32*)0x40000C8 // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(const volatile u32*)0x40000D4 // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
    const volatile void *src;
    const volatile void *dst;
    unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define LEFT 0
#define RIGHT 1

#define REG_DISPCNT *(u16*) 0x4000000
#define REG_VIDEO_PAGE1 0x6000000
#define REG_VIDEO_PAGE2 0x600A000
#define PALETTE_ADDRESS (u16*)0x5000000
#define COLOR(r,g,b) ((r) + ((g) << 5) + ((b) << 10))
#define MODE3 3
#define MODE4 4
#define BG2_ENABLE (1<<10)
#define DCNT_PAGE 0x0010

//Function prototypes
void setPixel3(int r, int c, u16 color);
void setPixel4(int r, int c, u8 clrid);
void drawRect(int r, int c, int width, int height, u16 color);
void drawHollowRect3(int r, int c, int width, int height, u16 color);
void drawHollowRect4(int r, int c, int width, int height, u8 clrid);
void plotLine3(int x0, int y0, int x1, int y1, u16 color);
void plotLine4(int x0, int y0, int x1, int y1, u8 clrid);
void waitForVBlank();
void fillScreen(u8 color);
void drawImage3(int r, int c, int width, int height, const u16* image);
void drawImage4(int r, int c, int width, int height, const u16* image);
void fillPortion(int r, int c, int width, int height, u8 color );
u16* PageFlip();
typedef struct {
    int r;
    int c;
} Point;

typedef struct {
    Point p;
    int direction;
} Enemy;