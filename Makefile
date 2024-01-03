vm: helper stack virtual_machine.c
	gcc -Wall virtual_machine.c -o virtual_machine.o -c
	gcc -g -o virt_machine virtual_machine.o helper.o stack.o

disassembler: helper disassembler.c
	gcc -g -o disassembler disassembler.c helper.o

stack: stack.c
	gcc -Wall stack.c -o stack.o -c

helper: helper.c
	gcc -Wall helper.c -o helper.o -c
