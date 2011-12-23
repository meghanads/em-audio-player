###############################################################################
# Makefile for the project AP_Ver2
###############################################################################

## General Flags
PROJECT = AP_Ver2
MCU = atmega32
TARGET = AP_Ver2.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=12000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=AP_Ver2.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main.o FAT32.o LCD_routines.o pcm_sample.o PWM_routines.o SD_routines.o SPI_routines.o UART_routines.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) AP_Ver2.hex AP_Ver2.eep AP_Ver2.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

FAT32.o: ../FAT32.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCD_routines.o: ../LCD_routines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pcm_sample.o: ../pcm_sample.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PWM_routines.o: ../PWM_routines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SD_routines.o: ../SD_routines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SPI_routines.o: ../SPI_routines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UART_routines.o: ../UART_routines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) AP_Ver2.elf dep/* AP_Ver2.hex AP_Ver2.eep AP_Ver2.lss AP_Ver2.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

