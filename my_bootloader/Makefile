
CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

CFLAGS 		:= -Iinclude -Wall -O2
CPPFLAGS   	:= -nostdinc -nostdlib -fno-builtin

objs := ./arch/arm920t/start.o 
objs += ./board/s3c2440/nandflash.o 
objs += ./board/s3c2440/norflash.o 
objs += ./board/s3c2440/sdram.o 
objs += ./board/s3c2440/uart.o
objs += ./lib/my_printf.o
objs += ./lib/string_utils.o
objs += ./lib/lib1funcs.o
objs += boot.o

boot.bin: $(objs)
	${LD} -Tboot.lds -o boot.elf $^ ./lib/libgcc.a
	${OBJCOPY} -O binary -S boot.elf $@
	${OBJDUMP} -D -m arm boot.elf > boot.dis
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.bin *.elf *.dis
	
