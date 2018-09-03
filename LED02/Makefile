all:
	arm-linux-gcc -c -o led02.o led02.c
	arm-linux-gcc -c -o start.o start.S
	arm-linux-ld -Ttext 0 start.o led02.o -o led02.elf
	arm-linux-objcopy -O binary -S led02.elf led02.bin
	arm-linux-objdump -D led02.elf > led02.dis
	
clean:
	rm *.bin *.o *.elf *.dis