GCC=../cross/bin/i586-elf-gcc -g
GAS=../cross/bin/i586-elf-as  -g

binboot: lambdaos.bin
	qemu-system-i386 -kernel lambdaos.bin

isoboot: lambdaos.iso
	qemu-system-i386 -cdrom lambdaos.iso

lambdaos.iso: lambdaos.bin grub.cfg
	mkdir -p isodir
	mkdir -p isodir/boot
	cp lambdaos.bin isodir/boot/lambdaos.bin
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o lambdaos.iso isodir

OBJECTS=boot.s.o kernel.o gdt.o gdt.s.o idt.o idt.s.o driver/vga.o lib/str.o lib/mem.o
lambdaos.bin: $(OBJECTS) linker.ld
	$(GCC) -T linker.ld -o lambdaos.bin -ffreestanding -O2 -nostdlib \
		$(OBJECTS) -lgcc

boot.s.o: boot.s
	$(GAS) boot.s -o boot.s.o

gdt.s.o: gdt.s
	$(GAS) gdt.s -o gdt.s.o

idt.s.o: idt.s
	$(GAS) idt.s -o idt.s.o

kernel.o: kernel.c driver/vga.h lib/str.h
	$(GCC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

lib/str.o: lib/str.c
	$(GCC) -c lib/str.c -o lib/str.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

lib/mem.o: lib/mem.c
	$(GCC) -c lib/mem.c -o lib/mem.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

driver/vga.o: driver/vga.c driver/vga.h lib/str.h
	$(GCC) -c driver/vga.c -o driver/vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gdt.o: gdt.c
	$(GCC) -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

idt.o: idt.c lib/mem.h
	$(GCC) -c idt.c -o idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

clean:
	rm -f *.o lambdaos.bin lambdaos.iso lib/*.o driver/*.o
	rm -rf isodir
