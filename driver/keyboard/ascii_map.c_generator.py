special_chars = (
	('BACKTICK', '`', '~'),
	('1', '1', '!'),
	('2', '2', '@'),
	('3', '3', '#'),
	('4', '4', '$'),
	('5', '5', '%'),
	('6', '6', '^'),
	('7', '7', '&'),
	('8', '8', '*'),
	('9', '9', '('),
	('0', '0', ')'),
	('MINUS', '-', '_'),
	('EQUALS', '=', '+'),
	('OPEN_BRACKET', '[', '{'),
	('CLOSE_BRACKET', ']', '}'),
	('BACKSLASH', '\\\\', '|'),
	('SEMICOLON', ';', ':'),
	('SINGLE_QUOTE', '\\\'', '"'),
	('COMMA', ',', '<'),
	('PERIOD', '.', '>'),
	('SLASH', '/', '?'),
	('SPACE', ' ', ' ')
)

print '#include "ps2/key_constants.h"'
print '#include "ascii_map.h"'
print 'bool kbd_shift = false;'
print 'char ascii_map(unsigned int c) {'
print "\tif (c == KBD_KEY_ENTER) {"
print "\t\treturn '\\n';"
for t in special_chars:
	print "\t} else if (c == KBD_KEY_"+t[0]+") {"
	print "\t\tif (kbd_shift) return '"+t[2]+"'; else return '"+t[1]+"';"
for char in [chr(i) for i in range(ord('A'), ord('Z')+1)]:
	print "\t} else if (c == KBD_KEY_"+char+") {"
	print "\t\tif (kbd_shift) return '"+char+"'; else return '"+char.lower()+"';"
print "\t} else {"
print "\t\treturn '\\0';"
print "\t}"
print "}"