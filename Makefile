GCC=../crosscompile/install/bin/i686-elf-gcc -ggdb3
GAS=../crosscompile/install/bin/i686-elf-as  -ggdb3
QEMU_OPTS=-soundhw pcspk #-s -S

# For an explanation of how this makefile works, see http://wiki.osdev.org/Makefile

SRCGEN   := ./isr.c ./driver/keyboard/ascii_map.c   ./driver/keyboard/ps2/keymap.c ./driver/keyboard/ps2/key_constants.h
# $(sort ...) is needed to remove duplicates (files in both $(SRCGEN) and the filesystem)
SRCFILES := $(sort $(shell find . -type f -name '*.c') $(SRCGEN))
ASMGEN   := ./isr.s ./irq.s
ASMFILES := $(sort $(shell find . -type f -name '*.s') $(ASMGEN))
HDRFILES := $(shell find . -type f -name '*.h')
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES)) $(patsubst %.s,%.s.o,$(ASMFILES))
DEPFILES := $(patsubst %.c,%.d,$(SRCFILES))

.PHONY: binboot isoboot htmldocs clean

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
	
lambdaos.bin: $(OBJFILES) linker.ld
	$(GCC) -T linker.ld -o lambdaos.bin -ffreestanding -O2 -nostdlib \
		$(OBJFILES) -lgcc

-include $(DEPFILES)

%.s.o: %.s Makefile
	@$(GAS) $< -o $@
	
%.o: %.c Makefile
	@$(GCC) -MMD -MP -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

isr.s: isr.s_generator.py isr_exceptions.py
	python isr.s_generator.py > isr.s

isr.c:isr.c_generator.py isr_exceptions.py
	python isr.c_generator.py > isr.c

irq.s: irq.s_generator.py
	python irq.s_generator.py > irq.s

driver/keyboard/ascii_map.c: driver/keyboard/ascii_map.c_generator.py
	python driver/keyboard/ascii_map.c_generator.py > driver/keyboard/ascii_map.c

driver/keyboard/ps2/keymap.c driver/keyboard/ps2/key_constants.h: driver/keyboard/ps2/keymap.c_generator.py driver/keyboard/ps2/keymap-set1 driver/keyboard/ps2/key-constants
	python driver/keyboard/ps2/keymap.c_generator.py

htmldocs: Readme.html
	
Readme.html: Readme.md
	markdown Readme.md > Readme.html

clean:
	rm -f $(OBJFILES)
	rm -f $(DEPFILES)
	rm -f *.pyc
	rm -f Readme.html
	rm -rf isodir
	rm -f lambdaos.bin lambdaos.iso
	rm -f $(SRCGEN) $(ASMGEN)
	
