GCC=../cross/bin/i586-elf-gcc -ggdb3
GAS=../cross/bin/i586-elf-as  -ggdb3
QEMU_OPTS=#-s -S

binboot: lambdaos.bin
	qemu-system-i386 -kernel lambdaos.bin $(QEMU_OPTS)

isoboot: lambdaos.iso
	qemu-system-i386 -cdrom lambdaos.iso $(QEMU_OPTS)

lambdaos.iso: lambdaos.bin grub.cfg
	mkdir -p isodir
	mkdir -p isodir/boot
	cp lambdaos.bin isodir/boot/lambdaos.bin
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o lambdaos.iso isodir

OBJECTS=boot.s.o kernel.o gdt.o gdt.s.o idt.o idt.s.o isr.o isr.s.o irq.o irq.s.o driver/vga.o driver/timer.o lib/str.o lib/mem.o lib/printf.o
lambdaos.bin: $(OBJECTS) linker.ld
	$(GCC) -T linker.ld -o lambdaos.bin -ffreestanding -O2 -nostdlib \
		$(OBJECTS) -lgcc

boot.s.o: boot.s
	$(GAS) boot.s -o boot.s.o

gdt.s.o: gdt.s
	$(GAS) gdt.s -o gdt.s.o

idt.s.o: idt.s
	$(GAS) idt.s -o idt.s.o

isr.s.o: isr.s
	$(GAS) isr.s -o isr.s.o

isr.s: isr.s_generator.py isr_exceptions.py
	python isr.s_generator.py > isr.s

isr.c:isr.c_generator.py isr_exceptions.py
	python isr.c_generator.py > isr.c

isr.o: isr.c isr.h idt.h lib/printf.h kernel.h
	$(GCC) -c isr.c -o isr.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

irq.s: irq.s_generator.py
	python irq.s_generator.py > irq.s

irq.s.o: irq.s
	$(GAS) irq.s -o irq.s.o

irq.o: irq.c isr.h idt.h
	$(GCC) -c irq.c -o irq.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

kernel.o: kernel.c driver/vga.h lib/str.h lib/printf.h
	$(GCC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

lib/str.o: lib/str.c
	$(GCC) -c lib/str.c -o lib/str.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

lib/mem.o: lib/mem.c
	$(GCC) -c lib/mem.c -o lib/mem.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

lib/printf.o: lib/printf.c
	$(GCC) -c lib/printf.c -o lib/printf.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

driver/vga.o: driver/vga.c driver/vga.h lib/str.h kernel.h
	$(GCC) -c driver/vga.c -o driver/vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

driver/timer.o: driver/timer.c irq.h kernel.h
	$(GCC) -c driver/timer.c -o driver/timer.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gdt.o: gdt.c
	$(GCC) -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

idt.o: idt.c lib/mem.h
	$(GCC) -c idt.c -o idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

htmldocs: Readme.html
	
Readme.html: Readme.md
	markdown Readme.md > Readme.html

clean:
	rm -f *.o lambdaos.bin lambdaos.iso lib/*.o driver/*.o
	rm -f isr.s isr.c isr_exceptions.pyc # Auto-generated by isr.*_generator.py
	rm -f irq.s
	rm -f Readme.html
	rm -rf isodir
