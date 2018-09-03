all:
	arm-linux-gcc -c -o led_key.o led_key.c
	arm-linux-gcc -c -o start.o start.S
	arm-linux-ld -Ttext 0 start.o led_key.o -o led_key.elf
	arm-linux-objcopy -O binary -S led_key.elf led_key.bin
	arm-linux-objdump -D led_key.elf > led_key.dis

clean:
	rm *.o *.elf *.bin *.dis