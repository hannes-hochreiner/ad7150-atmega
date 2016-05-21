gcc_flags_common = -g -O3 -ffreestanding -fno-exceptions -std=c99
gcc_flags_atmega = -mmcu=atmega328p -DF_CPU=1000000UL
avrdude_flags_common = -c buspirate -P /dev/buspirate
avrdude_flags_atmega = -p m328p

atmega_files = src/main.c \
							 src/twi.c src/twi.h \
							 src/sermon.c src/sermon.h

build: bld/atmega.o

bld:
	mkdir bld

bld/atmega.o: bld Makefile $(atmega_files)
	avr-gcc $(gcc_flags_common) $(gcc_flags_atmega) -o $@ $(filter %.c, $(atmega_files))

%.hex: %.o
	avr-objcopy -O ihex $< $@

flash: bld/atmega.hex
	avrdude $(avrdude_flags_common) $(avrdude_flags_atmega) -U flash:w:$<
