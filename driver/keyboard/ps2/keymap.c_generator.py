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
			code += '\t\t\t\t\tvga_writestring("'+ktree[byte][0].replace('\\','\\\\').replace('"','\\"')+" "+ktree[byte][1]+'");\n'
			code += '\t\t\t\t\tlevel = 0;\n'
		code += '\t\t\t\t\tbreak;\n'
	code += '\t\t\t\tdefault:\n'
	code += '\t\t\t\t\tkernel_panic("Keyboard driver bad state: level %d, unknown keychar");\n' % level
	code += '\t\t\t}\n'
	code += '\t\t\tbreak;\n'
	code += morecode
	return code
driver.levelcount = 0

print '#include "keymap.h"'
print '#include "../../vga.h"'
print '#include "../../../kernel.h"'
print 'void keyboard_kmap(unsigned char byte) {'
print '\tstatic unsigned int level = 0;'
print '\tswitch(level) {'
print driver(0, tree)
print '\t\tdefault:'
print '\t\t\tkernel_panic("Keyboard driver bad state: Invalid level");'
print '\t}'
print '}'
