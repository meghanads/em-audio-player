
//	Main File

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "SPI_routines.h"
#include "SD_routines.h"
#include "UART_routines.h"

#include "FAT32.h"
#include "options.h"
#include "PWM_routines.h"
#include "LCD_routines.h"
#include "KEY_routines.h"



// GLOBLE...

//Temparary
unsigned char CURR_FILE[11];

//Playing file status
unsigned char PLAY_FILE[11]; 
unsigned char STOPPED;	// if song is stopped


//functions
void LCD_clear(void);
unsigned char PLAY_SONG (unsigned char *fileName);

// Globle var
//unsigned long Gcluster;	// globle cluster



void init_ports(void)
{
	PORTA = 0xFF;
	DDRA  = 0xFF;	//DAC out


	
	PORTB = 0xEF;//0b11110011;
	DDRB  = 0xBF;//0b10101110; //MISO line i/p, rest o/p

	PORTC = 0x00;	// LCD - control & JTAG
	DDRC  = 0x0F;	// SW1 - PC6;	SW2 - PC7



	PORTD = 0x00;	// LCD - DATA
	DDRD = 0xFF;

};


//call this routine to initialize all peripherals
void init_devices(void)
{
	 cli();  //all interrupts disabled
	 init_ports();
	 init_spi();

	 LCD_init();



	 MCUCR = 0x00;	//MCU power management
	 GICR  = 0x00;	//General Interrupt control Reg.

	 TIMSK = 0x00; //timer interrupt sources
 
};


//========================= Main ====================================

int main()
{
  
	unsigned char option, error, data, FAT32_active, fname="PP1 WAV";
	unsigned int i;
	unsigned char fileName[13];

	_delay_ms(100);  //delay for VCC stabilization

	init_devices();




	LCD_DisplayString_F(1,1," *AUDIO PLAYER* ");

	LCD_DisplayString_F(2,1,"EMBEDDED PROJECT");

	

	cardType = 0;

	for (i=0; i<10; i++)
	{
		error = SD_init();
		if(!error) break;
	}

	if(error)
	{
		if(error == 1) LCD_DisplayString_F(1,1," SDcard ND ");
	

		while(1);  //wait here forever if error in SD init 
	}


	SPI_HIGH_SPEED_6M;	//SCK - 6 MHz
	_delay_ms(1);		//some delay


	FAT32_active = 1;
	error = getBootSectorData (); //read boot sector and keep necessary data in global variables
	if(error) 	
	{
	
		FAT32_active = 0;
	}

	while(1)
	{

		_delay_ms(2000);

		LCD_clear();


// UPPER PART IS WORKING FINE..


		//Finding Files ...

		unsigned char flag;

		flag = GET_LIST;

		unsigned long cluster, sector, firstSector, firstCluster, nextCluster;
		struct dir_Structure *dir;
		unsigned int i;
		unsigned char j;


RESTART:
		cluster = rootCluster; //root cluster

		while(1)
		{
			firstSector = getFirstSector (cluster);

			for(sector = 0; sector < sectorPerCluster; sector++)
			{
				SD_readSingleBlock (firstSector + sector);
	

				for(i=0; i<bytesPerSector; i+=32)
				{
					dir = (struct dir_Structure *) &buffer[i];

					if(dir->name[0] == EMPTY) //indicates end of the file list of the directory
					{
					
						break;   
					}
					if((dir->name[0] != DELETED) && (dir->attrib != ATTR_LONG_NAME))
					{
					
						for(j=0; j<11; j++)
						{
							CURR_FILE[j] = dir->name[j];
						
						}

						// Get KEY options HERE
						
					
				//	LCD_clear();
					LCD_DisplayString_F(1,1,CURR_FILE);
					LCD_DisplayString_F(2,1,"Play List...    ");

						while(1)
						{

							if(KEY1_PRESSED/*option == '1'*/)
							{
								_delay_ms(100);

								while(KEY1_PRESSED);
								// PLAY CURR_FILE (NEW)

								
								LCD_clear();
								LCD_DisplayString_F(1,1,CURR_FILE);
								LCD_DisplayString_F(2,1,"Playing...      ");

								STOPPED = 0;

								PLAY_SONG(CURR_FILE);

								LCD_clear();

								if(STOPPED)
								{
									goto RESTART ;
								
								}
										
								
								break;
							}

							if(KEY2_PRESSED/*option =='2'*/)
							{
								_delay_ms(100);
								while(KEY2_PRESSED);
								LCD_clear();
								// GO TO NXT SONG AND LIST IT
								
								break;
							}
							
								// WRONG KEY, STAY HERE FOREVER

						}
							
					
						
					}
				}
			}

		

			if(!STOPPED)
			{		
				if(cluster > 0x0ffffff6)
				{ break;} 
				if(cluster == 0) 
				{
					LCD_clear();
					LCD_DisplayString_F(1,1," Cluster ERROR  ");
					LCD_DisplayString_F(2,1,"  ???????????  ");
					break;
				}
			}
	}

}


// I am NOT comming HERE




	return 0;
};


void LCD_clear(void)
{

	LCD_DisplayString_F(1,1,"                ");

	LCD_DisplayString_F(2,1,"                ");

};



unsigned char PLAY_SONG (unsigned char *fileName)
{
		struct dir_Structure *dir;
		unsigned long cluster, byteCounter = 0, fileSize, firstSector;
		unsigned int k;
		unsigned char j, error;

		
		dir = findFiles (GET_FILE, fileName); //get the file location
		if(dir == 0)
		{
			LCD_clear();

			LCD_DisplayString_F(1,1,"FILE NOT FOUND");

			LCD_DisplayString_F(2,1," ????????  ");

			 
		  	return (0);
		 }

		

		cluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;

		fileSize = dir->fileSize;

		
		while(1)
		{
		  firstSector = getFirstSector (cluster);

		  for(j=0; j<sectorPerCluster; j++)
		  {
		    SD_readSingleBlock(firstSector + j);
    
			for(k=0; k<512; k++)
		    {
				// Sending to UART
		     	// transmitByte(buffer[k]);

			  	// Sending to DAC
				PORTA = buffer[k];

				if(KEY1_PRESSED)
				{
					// PAUSE/CONTINUE

					_delay_ms(50);
					while(KEY1_PRESSED);

					LCD_DisplayString_F(2,1,"Paused...       ");
					while(KEY1_OPEN)
					{	// iF STOP PRESSED
						
						if(KEY2_PRESSED)
						{
							_delay_ms(30);

							while(KEY2_PRESSED);

							LCD_clear();
							STOPPED = 1;
							return 0;
						}


					}

					_delay_ms(20);

					while(KEY1_PRESSED);
					_delay_ms(20);

					while(KEY1_PRESSED);
					LCD_DisplayString_F(2,1,"Playing...      ");
				}

				if(KEY2_PRESSED)
				{
					// STOP and LIST NEXT
					
					_delay_ms(50);

					while(KEY2_PRESSED);

					LCD_clear();
					STOPPED = 1;
					return 0;

					

				}
	
				_delay_ms(.125);

		      if ((byteCounter++) >= fileSize ) return 0;
		    }
		  }
		  cluster = getSetNextCluster (cluster, GET, 0);
		  if(cluster == 0)
		   {
		   		LCD_clear();
		   		LCD_DisplayString_F(1,1,"  ERROR:CLUSTER ");

				LCD_DisplayString_F(2,1,"   ???????    ");
				return 0;
		   }
		}
		return 0;
};

