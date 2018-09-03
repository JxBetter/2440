all:
	arm-linux-gcc -c -o led01.o led01.S
	arm-linux-ld -Ttext 0 led01.o -o led01.elf
	arm-linux-objcopy -O binary -S led01.elf led01.bin
	arm-linux-objdump -D led01.elf > led01.dis
	
clean:
	rm *.bin *.o *.elf