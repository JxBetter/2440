objs = start.o uart.o init.o nandflash.o main.o interrupt.o timer.o norflash.o my_printf.o string_utils.o lib1funcs.o

objs += lcd/lcd.o
objs += lcd/draw.o
objs += lcd/lcd_4.3.o
objs += lcd/lcd_test.o
objs += lcd/font_10x18.o
objs += lcd/lcd_controller.o
objs += lcd/s3c2440_lcd_controller.o

all: $(objs)
	#arm-linux-ld -Ttext 0 -Tdata 0x30000000  start.o led.o uart.o init.o main.o -o sdram.elf
	arm-linux-ld -T lcd.lds $^ libgcc.a -o lcd.elf
	arm-linux-objcopy -O binary -S lcd.elf lcd.bin
	arm-linux-objdump -D lcd.elf > lcd.dis
clean:
	rm *.bin *.o *.elf *.dis
	
%.o : %.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o $@ $<

%.o : %.S
	arm-linux-gcc -march=armv4 -c -o $@ $<
	
