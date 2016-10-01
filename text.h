typedef volatile unsigned char  vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int   vu32;

extern const unsigned char fontdata_6x8[12288];

void drawChar(int row, int col, char ch, unsigned char color);
void drawString(int row, int col, char *str, unsigned char color);

