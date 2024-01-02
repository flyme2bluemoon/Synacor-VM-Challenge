virtual_machine: virtual_machine.c stack.c
	gcc -Wall virtual_machine.c -o virtual_machine.o -c
	gcc -Wall stack.c -o stack.o -c
	gcc -o virt_machine virtual_machine.o stack.o
