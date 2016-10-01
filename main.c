//Color
#define BLACK   COLOR(0,0,0)
#define WHITE   COLOR(31, 31, 31)
#define RED     COLOR(31,0,0)
#define GREEN   COLOR(0, 31, 0)
#define BLUE    COLOR(0,0,31)
#define YELLOW  COLOR(31,31,0)
#define CYAN    COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define GRAY    COLOR(25, 25, 25)

//Button
#define BUTTON_ADDRESS *(volatile unsigned short*) 0x4000130
#define BUTTON_DOWN_NOW(key) (~BUTTON_ADDRESS & (key))

#define BUTTON_A 1
#define BUTTON_B (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START (1<<3)
#define BUTTON_RIGHT (1<<4)
#define BUTTON_LEFT (1<<5)
#define BUTTON_UP (1<<6)
#define BUTTON_DOWN (1<<7)
#define BUTTON_R (1<<8)
#define BUTTON_L (1<<9) 
#define NULL 0


#include "mylib.h"
#include "text.h"
#include "images.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void splashScreen();
void randomMap(u8 color);
void convertMap(u8 color);
void setup();
void fillOneBox(int r, int c, int width, int height, u8 color);
void randomMapHelper( int r, int c, int length, int direction );
void drawPenguine(int r, int c, int direction, int oldDirection );
int canMove(Point *current, int direction);
void drawMap( int r, int c, int mapR, int mapC );
int validPosition( int r, int c );
void randomTarget( );
void randomEnemies();
void drawEnemies( int displayR, int r);
void updateEnemies();
int checkEnemyCollisions();
static char * map; // [rows][columns]
static unsigned short * mapPixels;
//Map constants-- needs to be divisible by 2.
static const int MAP_COLUMN_WIDTH_PIXELS = 8;
static const int MAP_ROW_WIDTH_PIXELS = 8;
static int columns;
static int rows;
static int mapWidthPixels;
static int mapHeightPixels;
static int currentRow;
static int currentColumn;
static int numEnemies;
static Enemy * enemies;
#define U 0
#define D 1
#define L 2
#define R 3
#define OBSTACLE_TYPE_WALL 1
#define OBSTACLE_TYPE_TARGET 2
#define OBSTACLE_TYPE_ENEMY 3
extern u16 *videoBuffer;
extern u16 *backBuffer;

int main(){
    REG_DISPCNT = MODE4 | BG2_ENABLE;
    u8 color = 2;
    columns = 21;
    rows = 35;
    numEnemies = 5;
    mapWidthPixels = MAP_COLUMN_WIDTH_PIXELS * columns;
    mapHeightPixels = MAP_ROW_WIDTH_PIXELS * rows;
    randomMap(color);
    randomEnemies();
    setup();
    splashScreen();
    int c = 0;
    int r = 0;
    int displayR = 0;
    int displayC = 0;

    //Initalize Penguine information
    int direction = U;
    int penguineR = rand()%140 + 10;
    int penguineC = rand()%120;
    while (!validPosition( penguineR + displayR - r, penguineC*2 )){
      penguineR = rand()%140 + 10;
      penguineC = rand()%120;
    }
    int penguineInitialR = penguineR;

    drawMap( r, c, displayR, displayC );
    drawPenguine( penguineR, penguineC, direction, direction );
    drawEnemies(displayR, r);
    
    int level = 1;
    Point current;
    int foundTarget = 0;
    while(1){
        int oldDirection = direction;
        currentRow = (penguineR + displayR - r)/MAP_ROW_WIDTH_PIXELS;
        currentColumn = penguineC*2/MAP_COLUMN_WIDTH_PIXELS;
        if (BUTTON_DOWN_NOW(BUTTON_SELECT)){
          randomMap(color);
          randomEnemies();
          splashScreen();
          r = 0;
          c = 0;
          level = 0;
          displayR = 0;
          displayC = 0;
          penguineR = rand()%100 + 40;
          penguineC = rand()%50;
          while (!validPosition( penguineR + displayR - r, penguineC*2 )){
            penguineR = rand()%140 + 10;
            penguineC = rand()%120;
          }
          penguineInitialR = penguineR;
        }    
        
        if(BUTTON_DOWN_NOW(BUTTON_RIGHT)) {
            direction = R;
            current.r = penguineR + displayR - r;
            current.c = penguineC*2;
            int can = canMove(&current, R);
            if (can){
              if (can == OBSTACLE_TYPE_TARGET){
                foundTarget = 1;
              }
              penguineC += 1;
            }
        }
        
        if(BUTTON_DOWN_NOW(BUTTON_LEFT)) {
            direction = L;
            current.r = penguineR + displayR - r;
            current.c = penguineC*2;
            int can = canMove(&current, L);
            if (can){
              if (can == OBSTACLE_TYPE_TARGET){
                foundTarget = 1;                
              }
              penguineC -= 1;
            }         
        }
        
        if(BUTTON_DOWN_NOW(BUTTON_UP)) {
            direction = U;
            current.r = penguineR + displayR - r;
            current.c = penguineC*2;
            int can = canMove(&current, U);
            if (can){
              if (can == OBSTACLE_TYPE_TARGET){
                foundTarget = 1;                
              }
              displayR -= 1;
            }  
        }
        if(BUTTON_DOWN_NOW(BUTTON_DOWN)) {
            direction = D;
            current.r = penguineR + displayR - r;
            current.c = penguineC*2;
            int can = canMove(&current, D);
            if (can){
              if (can == OBSTACLE_TYPE_TARGET){
                foundTarget = 1;                
              }              
              displayR += 1;
              if ( r > 0 ){
                r--;
              }
            }              
        }
        if ( penguineC > mapWidthPixels /2 - SIDER_WIDTH){
            penguineC = mapWidthPixels/2 - SIDER_WIDTH;
        } else if ( penguineC <  MAP_COLUMN_WIDTH_PIXELS/2){
            penguineC = MAP_COLUMN_WIDTH_PIXELS/2;
        }

        if (displayR > (mapHeightPixels - penguineInitialR - FRONT_HEIGHT)){
          displayR = (mapHeightPixels - penguineInitialR - FRONT_HEIGHT);
        } else if (displayR < 0) {
          displayR = 0;
          if ( r < ( penguineInitialR - MAP_ROW_WIDTH_PIXELS )){
            r++;
          }
        }

        if ( foundTarget ){
          foundTarget = 0;
          level++;
          randomMap(color);
          randomEnemies();
          displayR = 0;
          displayC = 0;
          penguineR = rand()%140 + 10;
          penguineC = rand()%80;
          r = 0;
          c = 0;
          while (!validPosition( penguineR + displayR - r, penguineC*2 )){
            penguineR = rand()%100 + 30;
            penguineC = rand()%80;
          }
          penguineInitialR = penguineR;          
        }
        updateEnemies();
        if( checkEnemyCollisions(penguineR + displayR - r, penguineC*2 ) ){
          fillScreen(0);
          char * gameOver = "Game Over";
          char * restart = "Press Start to restart";
          drawString(20, 50, gameOver, 3);
          drawString(40, 50, restart, 3);
          char livestext[] = "Level: ";
          char *buffer = malloc(5*sizeof(char));
          sprintf (buffer, "%d", level);
          char *lives_str = strcat(livestext, buffer); // concatenate the str and the amount
          drawString(130, 100, lives_str, 20);
          free(buffer);
          PageFlip();
          while(!BUTTON_DOWN_NOW(BUTTON_START)){}
          foundTarget = 0;
          level = 1;
          randomMap(color);
          randomEnemies();
          displayR = 0;
          displayC = 0;
          penguineR = rand()%140 + 10;
          penguineC = rand()%80;
          r = 0;
          c = 0;
          while (!validPosition( penguineR + displayR - r, penguineC*2 )){
            penguineR = rand()%100 + 30;
            penguineC = rand()%80;
          }
          penguineInitialR = penguineR; 
        }
        waitForVBlank();

        drawMap( r, c, displayR, displayC );
        drawPenguine( penguineR, penguineC, direction, oldDirection );
        drawEnemies( displayR, r);
        char livestext[] = "Level: ";
        char *buffer = malloc(5*sizeof(char));
        sprintf (buffer, "%d", level);
        char *lives_str = strcat(livestext, buffer); // concatenate the str and the amount
        drawString(150, 175, lives_str, 3);
        
        char columntext[] = "Column: ";
        sprintf (buffer, "%d", currentColumn);
        lives_str = strcat(columntext, buffer);
        drawString(130, 175, lives_str, 3);

        char rowtext[] = "Row: ";
        sprintf (buffer, "%d", currentRow);
        lives_str = strcat(rowtext, buffer);
        drawString(110, 175, lives_str, 3);
        PageFlip();
        free(buffer);
    }

}

int checkEnemyCollisions( int r, int c ){
    Point current;
    current.r =r;
    current.c = c;
    if (   canMove(&current, U) == OBSTACLE_TYPE_ENEMY
        || canMove(&current, D) == OBSTACLE_TYPE_ENEMY
        || canMove(&current, L) == OBSTACLE_TYPE_ENEMY
        || canMove(&current, R) == OBSTACLE_TYPE_ENEMY
        )
    {
      return 1;
    }
    return 0;
}

void randomEnemies(){
  free(enemies);
  enemies = malloc(numEnemies*sizeof(Enemy));
  int i;
  for (i=0; i< numEnemies; i++){
    int r = rand()%mapHeightPixels;
    int c = rand()%(mapWidthPixels/2);
    while (!validPosition(r, c)){
      r = rand()%mapHeightPixels;
      c = rand()%(mapWidthPixels/2);
    }
    Point p = { .r = r, .c = c };
    int row = r/MAP_ROW_WIDTH_PIXELS;
    int column = c/MAP_COLUMN_WIDTH_PIXELS;
    if ( map[row*columns + column] != OBSTACLE_TYPE_WALL &&
        map[row*columns + column] != OBSTACLE_TYPE_TARGET ){
        map[row*columns + column] = OBSTACLE_TYPE_ENEMY;
    }
    int direction = rand()%4;
    Enemy e = { .p = p, .direction = direction };
    enemies[i] = e;
  }
}

int validPosition( int r, int c ){
    Point current;
    current.r =r;
    current.c = c;
    if (   canMove(&current, U)
        && canMove(&current, D)
        && canMove(&current, L)
        && canMove(&current, R)
        )
    {
      return 1;
    }
    return 0;
}

void splashScreen(){
    drawImage4( 0, 0, SPLASH_WIDTH, SPLASH_HEIGHT, Splash );
    while(!BUTTON_DOWN_NOW(BUTTON_START)){}
}

void setup(){
  //Copy palette
  DMA[3].src = &images_palette;
  DMA[3].dst = PALETTE_ADDRESS;
  DMA[3].cnt = IMAGES_PALETTE_SIZE | DMA_ON;
}

/**
* Check if the character can move in that direction.
**/
int canMove(Point *current, int direction){
  int r = current->r;
  int c = current->c;
  int ltR = 0, ltC = 0; //top left corner
  int lbR, lbC; //bottom left corner
  int rtR, rtC; //top right
  int rbR, rbC; //bottom right
  int width = 0, height = 0;
  int canMove = 1;
  switch (direction){
    case U:
      ltR = r - 1;
      ltC = c;
      width = BACK_WIDTH;
      height = BACK_HEIGHT;
      break;
    case D:
      ltR = r + 1;
      ltC = c;
      width = FRONT_WIDTH;
      height = FRONT_HEIGHT;
      break;
    case L:
      ltR = r;
      ltC = c - 1;
      width = SIDEL_WIDTH;
      height = SIDEL_HEIGHT;
      break;
    case R:
      ltR = r;
      ltC = c + 1;
      width = SIDER_WIDTH;
      height = SIDER_HEIGHT;
      break;
  }
  lbR = ltR + height/2;
  rtR = ltR;
  rbR = lbR;

  lbC = ltC;
  rtC = ltC + width;
  rbC = rtC;

  int row, column;
  row = ltR/MAP_ROW_WIDTH_PIXELS;
  column = ltC/MAP_COLUMN_WIDTH_PIXELS;
  if ( map[row*columns + column] == OBSTACLE_TYPE_WALL){
    return 0;
  } else if ( map[row*columns + column] ) {
    return map[row*columns + column];
  }

  row = lbR/MAP_ROW_WIDTH_PIXELS;
  column = lbC/MAP_COLUMN_WIDTH_PIXELS;
  if ( map[row*columns + column] == OBSTACLE_TYPE_WALL){
    return 0;
  } else if ( map[row*columns + column] ) {
    return map[row*columns + column];
  }

  row = rtR/MAP_ROW_WIDTH_PIXELS;
  column = rtC/MAP_COLUMN_WIDTH_PIXELS;
  if ( map[row*columns + column] == OBSTACLE_TYPE_WALL ){
    return 0;
  } else if ( map[row*columns + column] ) {
    return map[row*columns + column];
  }

  row = rbR/MAP_ROW_WIDTH_PIXELS;
  column = rbC/MAP_COLUMN_WIDTH_PIXELS;
  if ( map[row*columns + column] == OBSTACLE_TYPE_WALL){
    return 0;
  } else if ( map[row*columns + column] ) {
    return map[row*columns + column];
  }

  return canMove;
}

void drawPenguine(int r, int c, int direction, int oldDirection ){
    int width = 0;
    int height, i;
    switch(oldDirection){
      case U:
        width = BACK_WIDTH;
        height = BACK_HEIGHT;
        break;
      case R:
        width = SIDER_WIDTH;
        height = SIDER_HEIGHT;
        break;
      case L:
        width = SIDEL_WIDTH;
        height = SIDEL_HEIGHT;
        break;
      case D:
        width = FRONT_WIDTH;
        height = FRONT_HEIGHT;
        break;
    }
    for ( i = 0; i < height/2; i++){
        DMA[3].src = 0;
        DMA[3].dst = &backBuffer[(r+i)*240/2+c];
        DMA[3].cnt = width/2 | DMA_ON; 
    }

    switch(direction){
      case U:
        width = BACK_WIDTH;
        height = BACK_HEIGHT;
        for ( i = 0; i < height/2; i++){
          DMA[3].src = &back[i*width];
          DMA[3].dst = &backBuffer[(r+i)*240/2+c];
          DMA[3].cnt = width/2 | DMA_ON; 
        }
        break;
      case R:
        width = SIDER_WIDTH;
        height = SIDER_HEIGHT;
        for ( i = 0; i < height/2; i++){
          DMA[3].src = &sideR[i*width];
          DMA[3].dst = &backBuffer[(r+i)*240/2+c];
          DMA[3].cnt = width/2 | DMA_ON; 
        }
        break;
      case L:
        width = SIDEL_WIDTH;
        height = SIDEL_HEIGHT;
        for ( i = 0; i < height/2; i++){
          DMA[3].src = &sideL[i*width];
          DMA[3].dst = &backBuffer[(r+i)*240/2+c];
          DMA[3].cnt = width/2 | DMA_ON; 
        }
        break;
      case D:
        width = FRONT_WIDTH;
        height = FRONT_HEIGHT;
        for ( i = 0; i < height/2; i++){
          DMA[3].src = &front[i*width];
          DMA[3].dst = &backBuffer[(r+i)*240/2+c];
          DMA[3].cnt = width/2 | DMA_ON; 
        }
        break;
    }
}

void randomMap(u8 color){
  free(map);
  map = malloc( rows*columns * sizeof(char) );
  while (!map){
      map = malloc( rows*columns * sizeof(char) );
  }
  int i;

  //Clear memory
  DMA[3].src = 0;
  DMA[3].dst = map;
  DMA[3].cnt = rows*columns/2 | DMA_ON | DMA_SOURCE_FIXED;

  //Random interior
  for(i=0; i< columns*2; i++){
    randomMapHelper( rand()%rows, rand()%columns, rand()%2 + 1, rand()%4);  //randomMapHelper( 5, 5, 5, 2);
  }

  randomTarget();
  //Fill in the border
  for(i=0; i<columns; i++){
    map[i] = 1;
    map[columns*(rows-1) + i] = OBSTACLE_TYPE_WALL; 
  }

  for(i=0; i<rows; i++){
    map[columns*i] = 1;
    map[columns*i + (columns - 1)] = OBSTACLE_TYPE_WALL;
  }
  convertMap(color);
}

void randomMapHelper( int r, int c, int length, int direction ){
  int i;
  switch(direction){
    case U:
      if((r - length)>0){
        for(i=r; i>(r-length); i--){
          if (map[columns*i + c]){
            map[columns*i + c] = 0;
            break;
          }
          map[columns*i + c] = 1;
        }
        r = r-length;
      }
      break;
    case D:
      if((r + length) < rows){
        for(i=r; i<(r+length); i++){
          if (map[columns*i + c]){
            map[columns*i + c] = 0;
            break;
          }
          map[columns*i + c] = 1;
        }
        r = r+length;
      }
      break;
    case R:
      if((c + length) < columns ){
        for(i=c; i<(c+length); i++){
          if (map[columns*r + i]){
            map[columns*r + i] = 0;
            break;
          }
          map[columns*r + i] = 1;
        }
        c = c+length;
      }
      break;
    case L:
      if((c - length)>0){
        for(i=c; i>(c-length); i--){
          if (map[columns*r + i]){
            map[columns*r + i] = 0;
            break;
          }
          map[columns*r + i] = 1;
        }
        c = c-length;
      }
      break;
    default:
      break;
  }
}

void randomTarget(){
  map[(rand()%(rows-2) + 1)* columns + rand()%(columns - 2) + 1] = OBSTACLE_TYPE_TARGET;
}

void convertMap(u8 color){
  int mapLength = rows*columns*MAP_COLUMN_WIDTH_PIXELS*MAP_ROW_WIDTH_PIXELS;
  free(mapPixels);
  mapPixels = malloc( mapLength * sizeof(short) );
  while (!mapPixels){
      mapPixels = malloc( mapLength * sizeof(short) );
  }
  //Fill map pixels with 0s
  DMA[3].src = 0;
  DMA[3].dst = mapPixels;
  DMA[3].cnt = mapLength | DMA_ON;

  int i,j;
  for(i=0; i<rows; i++){
    for(j=0; j<columns; j++){
       int current = map[i*columns + j];
       if (current == OBSTACLE_TYPE_WALL){
        fillOneBox(
                    i*MAP_ROW_WIDTH_PIXELS, 
                    j*MAP_COLUMN_WIDTH_PIXELS, 
                    MAP_COLUMN_WIDTH_PIXELS, 
                    MAP_ROW_WIDTH_PIXELS, color
                  );
       } else if (current == OBSTACLE_TYPE_TARGET){
        fillOneBox(
                    i*MAP_ROW_WIDTH_PIXELS, 
                    j*MAP_COLUMN_WIDTH_PIXELS, 
                    MAP_COLUMN_WIDTH_PIXELS, 
                    MAP_ROW_WIDTH_PIXELS, 7
                  );
       }
    }
  }
  u16 *mapPixels16 = malloc( mapLength/2 *sizeof(short));
  while (!mapPixels16){
      mapPixels16 = malloc( mapLength/2 *sizeof(short) );
      splashScreen();
  }
  //Two pixels are drawn at the same time.
  //Need to combine 2 u8 color into 1 u16.
  for(i=0;i<mapLength; i++){
    u16 first = mapPixels[i] << 8;
    u16 * target = mapPixels16 + i/2;
    i++;
    *target =  first | mapPixels[i];
  }
  free(mapPixels);
  mapPixels = mapPixels16;
}

void fillOneBox(int r, int c, int width, int height, u8 color){
    int i,j;
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++){
          mapPixels[(r+i)*mapWidthPixels + c + j] = color;
        }
        // DMA[3].src = &color16;
        // DMA[3].dst = &mapPixels[(r+i)*mapWidthPixels+c];
        // DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
    }
}

void drawMap( int r, int c, int mapR, int mapC ){
    int i;
    
    DMA[3].src = 0;
    DMA[3].dst = backBuffer;
    DMA[3].cnt = 120*160 | DMA_ON | DMA_SOURCE_FIXED; 
    
    if ( mapWidthPixels/2 > 120){
      for ( i = 0; i < (160 - r); i++){
        DMA[3].src = &mapPixels[(i+mapR)*mapWidthPixels/2 + mapC];
        DMA[3].dst = &backBuffer[(r+i)*240/2+c];
        DMA[3].cnt = 120 | DMA_ON; 
      }
    } else {
      for ( i = 0; i < (160 - r); i++){
        DMA[3].src = &mapPixels[(i+mapR)*mapWidthPixels/2 + mapC];
        DMA[3].dst = &backBuffer[(r+i)*240/2+c];
        DMA[3].cnt = mapWidthPixels/2 | DMA_ON; 
      }
    }
}

void drawEnemies(int displayR, int r){
  int i;
  for(i=0; i< numEnemies; i++){
    Enemy e = enemies[i];
    int rCanvas = e.p.r - displayR + r;
    int cCanvas = e.p.c/2;
    if (rCanvas < 160 && cCanvas < (mapWidthPixels/2) && rCanvas > 0 && cCanvas > 0 ){
      fillPortion(rCanvas, cCanvas, 6, 6, 10 );
    }
  }
}

void updateEnemies(){
  int i;
  for(i=0; i< numEnemies; i++){
    Enemy e = enemies[i];
    int r = e.p.r;
    int c = e.p.c;
    int row = r/MAP_ROW_WIDTH_PIXELS;
    int column = c/MAP_COLUMN_WIDTH_PIXELS;
    map[row*columns + column] = 0;
    Point p = { .r = r, .c = c};
    int direction = e.direction;
    if (!canMove(&p, direction)){
      direction = rand()%4;
      while (!canMove(&p, direction)){
        direction = rand()%4;
      }
    }
    switch (direction){
        case U:
          r--;
          (enemies[i]).p.r = r;
          (enemies[i]).direction = U;
          break;
        case D:
          r++;
          (enemies[i]).p.r = r;
          (enemies[i]).direction = D;
          break;
        case L:
          c--;
          (enemies[i]).p.c = c;
          (enemies[i]).direction = L;
          break;
        case R:
          c++;
          (enemies[i]).p.c = c;
          (enemies[i]).direction = R;
          break;
    }
    row = r/MAP_ROW_WIDTH_PIXELS;
    column = c/MAP_COLUMN_WIDTH_PIXELS;
    if ( map[row*columns + column] != OBSTACLE_TYPE_WALL &&
        map[row*columns + column] != OBSTACLE_TYPE_TARGET ){
      map[row*columns + column] = OBSTACLE_TYPE_ENEMY;
    }
  }
}