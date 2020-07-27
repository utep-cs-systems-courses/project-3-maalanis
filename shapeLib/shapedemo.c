#include "msp430.h"
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"
#include "p2switches.h"
#include "abCircle.h"

const AbRect rect10 = {abRectGetBounds, abRectCheck, 10,10};

#define GREEN_LED BIT6

AbRectOutline fieldOutline = {
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2-10, screenHeight/2-10}
};

Layer layer1 = {
  (AbShape *) &circle8,
  {(screenWidth/2)-5, (screenHeight/2)+10},
  {0,0}, {0,0},
  COLOR_YELLOW,
  0
};



Layer fieldLayer = {
  (AbShape *) &fieldOutline,
  {(screenWidth/2),screenHeight/2},
  {0,0}, {0,0},
  COLOR_RED,
  &layer1,
};
/*
Layer layer1 = {
  (AbShape *&cicle8,
   ((screenW
};
*/
Layer layer0 ={
  (AbShape *)&rect10,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},
  COLOR_BLACK,
  &fieldLayer,
};
void abDrawPos(AbShape *shape, Vec2 *shapeCenter, u_int fg_color, u_int bg_color)
{
  u_char row, col;
  Region bounds;
  abShapeGetBounds(shape, shapeCenter, &bounds);
  lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1],
	      bounds.botRight.axes[0]-1, bounds.botRight.axes[1]-1);
  for (row = bounds.topLeft.axes[1]; row < bounds.botRight.axes[1]; row++) {
    for (col = bounds.topLeft.axes[0]; col < bounds.botRight.axes[0]; col++) {
      Vec2 pixelPos = {col, row};
      int color = abShapeCheck(shape, shapeCenter, &pixelPos) ?
	fg_color : bg_color;
      lcd_writeColor(color);
    }
}
}
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

MovLayer ml1 = {&layer1, {2,2}, 0};
MovLayer ml0 = {&layer0, {2,2}, &ml1};

void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for(; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for(axis = 0; axis <2; axis++) {
      if((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	 (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (2*velocity);
      }
    }
    ml->layer->posNext = newPos;
  }
  
}
void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);
  for(movLayer= movLayers; movLayer; movLayer = movLayer->next){
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos= l->posNext;
  }
  or_sr(8);
  
  for(movLayer = movLayers; movLayer; movLayer = movLayer->next) {
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1],
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for(row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for(col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for(probeLayer = layers; probeLayer;
	    probeLayer = probeLayer->next) {
	  if(abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break;
	  }
	}
	lcd_writeColor(color);
      }
    }
  }
}


u_int bgColor = COLOR_BLUE;
int redrawScreen = 1;

Region fieldFence;



int
main()
{

  P1DIR |= GREEN_LED;
  P1OUT |= GREEN_LED;
  configureClocks();
  lcd_init();
  p2sw_init(1);
  shapeInit();

  shapeInit();
  
  layerInit(&layer0);
  layerDraw(&layer0);

  
  
  layerGetBounds(&fieldLayer, &fieldFence);
  Vec2 rectPos = screenCenter;

  //clearScreen(COLOR_BLUE);
   drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);
  //shapeInit();
  // drawPixel(screenCenter.axes[1], screenCenter.axes[0], COLOR_BLACK);
  //abDrawPos((AbShape*)&rect10, &rectPos, COLOR_ORANGE, COLOR_BLUE);
  enableWDTInterrupts();
  or_sr(0x8);
  for(;;){
    while(!redrawScreen) {
      P1OUT &= ~GREEN_LED;
      or_sr(0x10);
    }
    P1OUT |= GREEN_LED;
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);
  }
}
void wdt_c_handler() {
  static short count = 0;
  P1OUT |= GREEN_LED;
  count ++;
  if(count == 15) {
    mlAdvance(&ml0, &fieldFence);
    if(p2sw_read())
      redrawScreen = 1;
    count =0;
  }
  P1OUT &= ~GREEN_LED;
    
  }


