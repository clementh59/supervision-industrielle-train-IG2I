all: common console file_parser trame communications gestionRessources commande_train coordination test_troncon

coordination: coordination.c file_parser.o communications.o gestionRessources.o console.o commande_train.o trame.o common.o
	gcc console.o common.o file_parser.o communications.o gestionRessources.o commande_train.o trame.o coordination.c -o  coordination.exe -pthread -lm

test_troncon: test_troncon.c file_parser.o communications.o gestionRessources.o console.o commande_train.o trame.o common.o
	gcc console.o common.o file_parser.o communications.o gestionRessources.o commande_train.o trame.o test_troncon.c -o  test_troncon.exe -pthread -lm

trame: trame.c
	gcc -c trame.c

communications: communications.c
	gcc -c communications.c

gestionRessources: gestionRessources.c
	gcc -c gestionRessources.c

commande_train: commande_train.c
	gcc -c commande_train.c

file_parser: file_parser.c
	gcc -c file_parser.c

console: console.c
	gcc -c console.c

common: ../common.c
	gcc -c ../common.c

clean:
	rm -f *.o *.exe
