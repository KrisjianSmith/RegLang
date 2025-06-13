FLAGS=-Wall --std=c99 

all: main.o token_list.o parse_tree.o
	gcc ${FLAGS} main.o token_list.o parse_tree.o -o RegLang 

parse_tree.o: parse_tree.c parse_tree.h token_list.h
	gcc ${FLAGS} -c parse_tree.c

token_list.o: token_list.c token_list.h
	gcc ${FLAGS} -c token_list.c

main.o:	main.c token_list.h parse_tree.h
	gcc ${FLAGS} -c main.c

clean:
	rm -f *.o RegLang
