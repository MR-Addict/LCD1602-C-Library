#include <units.h>
#include <delay.h>
#include <LCD.h>

code uint8_t Custom_Char1[]={0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00};	//自定义字符℃
code uint8_t Custom_Char2[]={0x00,0x0A,0x1F,0x1F,0x1F,0x1F,0x0E,0x00};	//自定义字符℃

void main(){
	LCD_Init();
	LCD_Set_Cursor(0,5);
	LCD_Print_Str("Hell");
	LCD_Print_Char(0x6f);
	LCD_Set_Cursor(1,7);
	LCD_Print_Str("Worl");
	LCD_Print_Char('d');
	/*
	LCD_Custom_Char(1,Custom_Char1);
	LCD_Custom_Char(2,Custom_Char2);
	LCD_Print_Custom_Char(1,13,1);
	LCD_Print_Custom_Char(1,12,2);*/
	while(1){
	}
}
