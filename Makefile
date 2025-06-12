FLAGS=-Wall --std=c99 

all: main.o token_list.o
	gcc ${FLAGS} main.o token_list.o -o RegLang 

token_list.o: token_list.c token_list.h
	gcc ${FLAGS} -c token_list.c

main.o:	main.c token_list.h
	gcc ${FLAGS} -c main.c

clean:
	rm -f *.o RegLang
