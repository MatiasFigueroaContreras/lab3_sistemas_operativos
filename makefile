FLAGS = -g -Wall

all: clean lab3 run

lab3: lab3.c funciones.o yearData.o
	gcc $(FLAGS) -o lab3 funciones.o yearData.o lab3.c

funciones.o: funciones.c funciones.h
	gcc $(FLAGS) -c funciones.c

yearData.o: yearData.c yearData.h
	gcc $(FLAGS) -c yearData.c

run:
	./lab3

clean:
	rm -f lab3 *.o