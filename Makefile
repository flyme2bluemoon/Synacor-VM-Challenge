vm: helper stack decoder virtual_machine.c
	gcc -Wall -o virt_machine virtual_machine.c helper.o stack.o decoder.o

disassembler: helper decoder disassembler.c
	gcc -Wall -o disassembler disassembler.c helper.o decoder.o

decoder: decoder.c
	gcc -Wall decoder.c -o decoder.o -c

stack: stack.c
	gcc -Wall stack.c -o stack.o -c

helper: helper.c
	gcc -Wall helper.c -o helper.o -c
