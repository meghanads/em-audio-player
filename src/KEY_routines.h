
#ifndef _KEY_
#define _KEY_



#define KEY1_PRESSED	!(PINC & 0x40)	//PC.6
#define KEY1_OPEN		(PINC & 0x40)

#define KEY2_PRESSED	!(PINC & 0x80)	//PC.7
#define KEY2_OPEN		(PINC & 0x80)




#endif
