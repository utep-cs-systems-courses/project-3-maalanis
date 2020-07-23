/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  //  fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  int x = 0;
  for(int i = 0; i< 30; i++){
    for(int j =60-x ; j>30; j--) {
      drawPixel(30+i, j, COLOR_GREEN);
    }
    x++;
  }


  //CODE FOR STAR
  
  for(int i = 150; i> 140; i--){
    drawPixel(60, i, COLOR_GREEN);
  }
  for(int i = 160; i> 150; i--) {
    drawPixel(60, i, COLOR_GREEN);
  }
  for(int i = 50; i<70; i++) {
    drawPixel(i, 150, COLOR_GREEN);

  }

  int sum = 1;
  for(int i = 0; i< 20; i++){
    drawPixel(50+sum, 140+sum, COLOR_GREEN);
    sum++;
  }

  sum = 1;
  for(int i = 0; i<20; i++){
    drawPixel(70-sum, 140+sum, COLOR_GREEN);
    sum ++;
  }
 
  
}
