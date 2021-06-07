all: common tester gestionnaire_ressource test_gestionnaire_ressource_simple

gestionnaire_ressource: common.o gestionnaire_ressource.c
	gcc common.o gestionnaire_ressource.c -o gestionnaire_ressource.exe -pthread -lm

test_gestionnaire_ressource_simple: common.o tester.o test_gestionnaire_ressource_simple.c
	gcc common.o tester.o test_gestionnaire_ressource_simple.c -o test_gestionnaire_ressource_simple.exe -pthread -lm

common: ../common.c
	gcc -c ../common.c

tester: tester.c
	gcc -c tester.c

clean:
	rm -f *.exe *.o
