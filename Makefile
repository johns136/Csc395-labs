PORT=/dev/tty.usbmodem14201

#LUFA_PATH=/Users/amylarson/Documents/__CodingandGit/_MaterialGit/RTES/lufa-LUFA-151115/LUFA
#VIRTUAL_SERIAL_PATH=/Users/amylarson/Documents/__CodingandGit/_MaterialGit/RTES/VirtualSerial

#PATH1 = export VIRTUAL_SERIAL_PATH=/Users/amylarson/Documents/__CodingandGit/_MaterialGit/RTES/VirtualSerial
#PATH2 = $(shell export LUFA_PATH=/Users/amylarson/Documents/__CodingandGit/_MaterialGit/RTES/lufa-LUFA-151115/LUFA)

#LP =  "export
#@echo $(VSP)
#@echo $(LP)

ifndef VIRTUAL_SERIAL_PATH
$(warning VIRTUAL_SERIAL_PATH must be set to use usb virtual serial)
else
CFLAGS+= -I$(VIRTUAL_SERIAL_PATH) -DVIRTUAL_SERIAL
LDFLAGS+=  -L$(VIRTUAL_SERIAL_PATH) -lVirtualSerial
endif

ifneq ($(filter program,$(MAKECMDGOALS)),$())
ifndef PORT
$(error PORT must be set to program device)
endif
endif

MCU=atmega32u4
CFLAGS+= -g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99
LDFLAGS+= -Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=main
OBJECT_FILES=buttons.o leds.o gpio.o scheduler.o main.o

#first: all
#	echo $(shell export LUFA_PATH=/Users/amylarson/Documents/__CodingandGit/_MaterialGit/RTES/lufa-LUFA-151115/LUFA)

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -C /usr/local/CrossPack-AVR-20131216/etc/avrdude.conf  -P $(PORT) -U flash:w:$(TARGET).hex
