leds.bin: leds.s
	arm-linux-gnueabihf-gcc-4.9 -g -c leds.s -o leds.o
	arm-linux-gnueabihf-ld -Ttext 0x87800000 leds.o -o leds.elf
	arm-linux-gnueabihf-objcopy -O binary -S -g leds.elf leds.bin
	arm-linux-gnueabihf-objdump -D leds.elf > leds.dis
clean:
	rm -f leds.dis leds.bin leds.elf leds.o