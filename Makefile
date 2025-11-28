C_SOURCES = $(wildcard src/kernel/*.c src/drivers/*.c)
HEADERS  = $(wildcard src/kernel/*.h src/drivers/*.h)
OBJ = ${C_SOURCES:.c=.o src/kernel/interrupt.o}

all : os.img

run : all
	qemu-system-x86_64 os.img

os.img : src/boot/boot.bin kernel.bin
	cat $^ > $@
	truncate -s 20480 $@

kernel.bin: src/boot/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

%.o : %.c ${HEADERS}
	gcc -m32 -ffreestanding -fno-pie -c $< -o $@
	# gcc -m32 -ffreestanding -c $< -o $@
%.o : %.asm
	nasm $< -f elf32 -o $@

%.bin : %.asm
	nasm $< -f bin -o $@

clean:
	rm -f *.bin *.o *.dis *.img *.map
	rm -rf src/kernel/*.o src/boot/*.bin src/drivers/*.o


