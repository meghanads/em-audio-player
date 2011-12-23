

// LCD routines

#include "LCD_routines.h"
#include <avr/io.h>
#include <util/delay.h>



// *********************************

// *** Initialize the LCD driver ***

// *********************************

void LCD_init(void)

{

  delay_ms(100); // wait for 100ms

  LCD_WriteCommand (0x38); // 8 data lines

  LCD_WriteCommand (0x06); // cursor setting

  LCD_WriteCommand (0x0f); // display ON

  LCD_WriteCommand (0x01); // clear LCD memory

  delay_ms (10); // 10ms delay after clearing LCD

}



// **********************************************

// *** Write a command instruction to the LCD ***

// **********************************************

void LCD_WriteCommand (unsigned char Command)

{


  SET_LCD_CMD; // Set LCD in command mode


  PORTD = Command; // Load data to port


  ENABLE_LCD; // Write data to LCD


  asm("nop");

  asm("nop");

  DISABLE_LCD; // Disable LCD

  _delay_ms(1); // wait for 1ms

}



// *****************************************

// *** Write one byte of data to the LCD ***

// *****************************************

void LCD_WriteData (unsigned char Data)

{

  SET_LCD_DATA; // Set LCD in data mode


  PORTD = Data; // Load data to port


  ENABLE_LCD; // Write data to LCD


  asm("nop");

  asm("nop");


  DISABLE_LCD; // Disable LCD

  _delay_ms(1); // wait for 1ms

}



// ************************************************************

// Display a string at the specified row and column, from FLASH

//*************************************************************

void LCD_DisplayString_F (char row, char column ,const unsigned char *string)

{

  LCD_Cursor (row, column);

  while (*string)

    LCD_WriteData(*string++);

}



// ***************************************************

// *** Position the LCD cursor at "row", "column". ***

// ***************************************************

void LCD_Cursor (char row, char column)

{

  switch (row)

  {

    case 1: LCD_WriteCommand (0x80 + column - 1); break;

    case 2: LCD_WriteCommand (0xc0 + column - 1); break;

    default: break;

  }

}



// ********************************************************

// **** Function for delay of 1 msec (appx.) at 16Mhz *****

// ********************************************************

void delay_ms(int miliSec) //for 16 Mhz crystal

{

    int i,j;

    for(i=0;i<miliSec;i++)

    for(j=0;j<1550;j++)

  {

   asm("nop");

   asm("nop");

  }

}


// ********************** END **************************





