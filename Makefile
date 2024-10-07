COMPILER = gcc -g
FLAGS = -std=c11
COMPILE = $(COMPILER) $(FLAGS)

out: pa0_Lee.o command.o
	$(COMPILE) pa0_Lee.o command.o -o mainexe

pa0_Lee.o: pa0_Lee.c
	$(COMPILE) -c pa0_Lee.c

command.o: command.c
	$(COMPILE) -c command.c
	

clean:
	rm pa0_Lee.o; rm command.o; rm mainexe