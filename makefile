.PHONY: run compile clean

run: shell
	@./shell
compile shell: main.o prompt.o general.o utils.o
	@gcc -o shell main.o prompt.o general.o utils.o
main.o: main.c prompt.h general.h
	@gcc -c main.c
prompt.o: prompt.c prompt.h
	@gcc -c prompt.c
general.o: general.c general.h utils.h
	@gcc -c general.c
utils.o: utils.c utils.h
	@gcc -c utils.c
clean:
	@rm -f shell *.o temp.txt