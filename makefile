.PHONY: run compile clean

run: shell
	@./shell
compile shell: main.o prompt.o general.o
	@gcc -o shell main.o prompt.o general.o
main.o: main.c prompt.h general.h
	@gcc -c main.c
prompt.o: prompt.c prompt.h
	@gcc -c prompt.c
general.o: general.c general.h
	@gcc -c general.c
clean:
	@rm -f shell *.o