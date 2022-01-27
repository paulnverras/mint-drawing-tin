#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()
#include <signal.h>     //signal()

#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "image.h"
#include "GUI_BMPfile.h"

void  Handler(int signo)
{
    //System Exit
	printf("\r\nHandler:exit\r\n");
	DEV_ModuleExit();

    exit(0);
}

int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    DEV_ModuleInit();
	//RED
	LCD_Init();
	LCD_Clear(WHITE);
		
	UBYTE *BlackImage;
    UDOUBLE Imagesize = LCD_WIDTH * LCD_HEIGHT *2;
	printf("Imagesize = %ld\r\n",Imagesize);
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
	
	Paint_NewImage(BlackImage,LCD_WIDTH,LCD_HEIGHT, 0, WHITE);
	Paint_Clear(WHITE);
	Paint_SetRotate(270);
	Paint_DrawString_EN(30, 10, "123", &Font24, YELLOW, RED);
	Paint_DrawString_EN(30, 34, "ABC", &Font24, BLUE, CYAN);
	Paint_DrawString_CN(50,180, "Î¢Ñ©µç×Ó",  &Font24CN, WHITE, RED);
	Paint_DrawRectangle(125, 10, 225, 58, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
	Paint_DrawLine(125, 10, 225, 58, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
	Paint_DrawLine(225, 10, 125, 58, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
	
	Paint_DrawCircle(150,100, 25, BLUE,  DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
	Paint_DrawCircle(180,100, 25, BLACK,    DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
	Paint_DrawCircle(210,100, 25, RED, DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
	Paint_DrawCircle(165,125, 25, YELLOW,   DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
	Paint_DrawCircle(195,125, 25, GREEN,   DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
	
	Paint_DrawImage(gImage_100X100, 10, 75,100, 100); 
	
	LCD_Display(BlackImage);
	
	DEV_Delay_ms(2000);
	GUI_ReadBmp_RGB("./pic/LCD_2inch.bmp",0,0);
	
	LCD_Display(BlackImage);
	//System Exit
	DEV_ModuleExit();
	return 0;
	
}

