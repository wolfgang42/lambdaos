from __future__ import print_function

constants = {}
with open('driver/keyboard/ps2/key-constants') as f:
	for line in f.readlines():
		data = line.strip().split("\t")
		bytestr = data[0]
		if len(data) == 1:
			const = data[0]
			asciiChar = data[0]
		elif len(data) == 2:
			const = data[1]
			asciiChar = data[0]
		elif len(data) == 3:
			const = data[1]
			asciiChar = data[2]
		constants[bytestr] = (
				const.replace(" ", '_').upper(), asciiChar
			)

tree = {}
with open('driver/keyboard/ps2/keymap-set1') as f:
	for line in f.readlines():
		(bytestr, command, direction) = line.strip().split("\t")
		bytes = bytestr.split(", ")
		pos = tree
		for byte in bytes:
			try:
				pos[byte]
			except KeyError:
				pos[byte] = {}
			prepos = pos
			pos = pos[byte]
		prepos[byte] = (command, direction)

def chartoconst(char):
	return 'KBD_CHAR_'+constants[char][0]

def driver(level, ktree):
	code = ""
	morecode = ""
	code += '\t\tcase %d:\n' % level
	code += '\t\t\tswitch (byte) {\n'
	for byte in ktree.keys():
		code += '\t\t\t\tcase %s:\n' % byte
		if type(ktree[byte]) == dict:
			driver.levelcount += 1
			code += '\t\t\t\t\tlevel = %d;\n' % driver.levelcount
			morecode += driver(driver.levelcount, ktree[byte])
		else:
			# TODO actually do something
			code += '\t\t\t\t\tkeyboard_event('+chartoconst(ktree[byte][0])+", "
			code += "KBD_ACT_"+ktree[byte][1].upper()+');\n'
			code += '\t\t\t\t\tlevel = 0;\n'
		code += '\t\t\t\t\tbreak;\n'
	code += '\t\t\t\tdefault:\n'
	code += '\t\t\t\t\tkernel_panic("Keyboard driver bad state: level %d, unknown keychar");\n' % level
	code += '\t\t\t}\n'
	code += '\t\t\tbreak;\n'
	code += morecode
	return code
driver.levelcount = 0

with open('driver/keyboard/ps2/keymap.c', 'w') as f:
	print("""#include "keymap.h"
#include "key_constants.h"
#include "ps2.h"
#include "../../vga.h"
#include "../../../kernel.h"
void keyboard_kmap(unsigned char byte) {
\tstatic unsigned int level = 0;
\tswitch(level) {""", file=f)
	print(driver(0, tree), file=f)
	print ("""\t\tdefault:
\t\t\tkernel_panic("Keyboard driver bad state: Invalid level");
\t}
}""", file=f)

i=0
with open('driver/keyboard/ps2/key_constants.h', 'w') as f:
	print("#define KBD_ACT_PRESSED 0", file=f)
	print("#define KBD_ACT_RELEASED 1", file=f)
	for char in constants:
		print ("#define "+chartoconst(char)+" "+str(i), file=f)
		i+=1
