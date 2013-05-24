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

lambdaos.bin: boot.o kernel.o gdt.o driver/vga.o lib/string.o linker.ld
	../cross/bin/i586-elf-gcc -T linker.ld -o lambdaos.bin -ffreestanding -O2 -nostdlib \
		boot.o kernel.o gdt.o driver/vga.o lib/string.o  -lgcc

boot.o: boot.s
	../cross/bin/i586-elf-as boot.s -o boot.o

kernel.o: kernel.c
	../cross/bin/i586-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

driver/vga.o: driver/vga.c
	../cross/bin/i586-elf-gcc -c driver/vga.c -o driver/vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gdt.o: gdt.c
	../cross/bin/i586-elf-gcc -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

clean:
	rm -f boot.o kernel.o lambdaos.bin lambdaos.iso
	rm -rf isodir
