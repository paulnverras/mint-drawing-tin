#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

void setup()
{
  Config_Init();
  LCD_Init();
  LCD_Clear(0xffff);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 180, WHITE);
  Serial.print("hi");
  Paint_DrawCircle(150,100, 25, BLUE,  DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
  

}
void loop()
{
  
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
