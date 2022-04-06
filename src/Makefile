mysh: shell.c interpreter.c shellmemory.c scheduler.c
	gcc -D FRS=$(frs) -D FRL=$(frl) -c shell.c interpreter.c shellmemory.c scheduler.c
	gcc -o mysh shell.o interpreter.o shellmemory.o scheduler.o

clean: 
	rm mysh; rm *.o


