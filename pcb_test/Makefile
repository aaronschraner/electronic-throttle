#microcontroller information
MCU=atmega2560
AVRDUDEMCU=m2560
OBJ2HEX=avr-objcopy
AVRDUDE=avrdude

CPPFILES=main.cpp
CC=avr-g++
HFILES=
CFLAGS=-g -Os -std=c++11 -Wall -Wno-reorder -mcall-prologues -mmcu=$(MCU) -DF_CPU=$(CPU_FREQ)
PROGRAMMER=usbtiny
TARGET=pcb_test

###fuse configuration###
#use 8MHz internal clock
#LFUSE=0xE2
#CPU_FREQ=8000000UL
#use 1MHz internal clock
#LFUSE=0x62
#CPU_FREQ=1000000UL
# use 16MHz ext. clock
LFUSE=0xFF
CPU_FREQ=16000000UL

HFUSE=0xD8
EFUSE=0xFD

build: $(CPPFILES) $(HFILES)
	$(CC) $(CFLAGS) $(CPPFILES) -o $(TARGET).out
	$(OBJ2HEX) -R .eeprom -O ihex $(TARGET).out $(TARGET).hex

upload: build
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -c $(PROGRAMMER) -U flash:w:$(TARGET).hex

fuse:
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -c $(PROGRAMMER) \
		-U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m


clean:
	rm -fv $(TARGET).out $(TARGET).hex

