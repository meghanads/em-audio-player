// LCD routines


// ************* LCD.h : Header file *************


#ifndef	_LCD_ROUTINES_
#define	_LCD_ROUTINES_





void LCD_init(void);

void LCD_WriteCommand (unsigned char CMD);

void LCD_WriteData (unsigned char Data);

void LCD_DisplayString_F(char row, char column, const unsigned char *string);

void LCD_Cursor(char row, char column);

void delay_ms(int miliSec);


#define ENABLE_LCD PORTC |= 0x02	//LCD- 6pin; uC-PC.2

#define DISABLE_LCD PORTC &= ~0x02


#define SET_LCD_DATA PORTC |= 0x01	//LCD - 4pin; uC-PC.1

#define SET_LCD_CMD PORTC &= ~0x01


//#define pushButton1_PRESSED !(PINB & 0x02)

//#define pushButton1_OPEN (PINB & 0x02)





#endif
