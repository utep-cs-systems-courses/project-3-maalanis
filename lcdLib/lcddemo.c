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

  drawStar();

}
