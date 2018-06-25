all: turing
turing : infoInput.o infoOutput.o main.o strcut.o
	gcc -pedantic -Wall -Wextra -o turing infoInput.o infoOutput.o main.o strcut.o

infoInput.o : infoInput.c
	gcc -std=c11 -pedantic -Wall -Wextra -c -o infoInput.o infoInput.c

infoOutput.o : infoOutput.c
	gcc -std=c11 -pedantic -Wall -Wextra -c -o infoOutput.o infoOutput.c

main.o : main.c
	gcc -std=c11 -pedantic -Wall -Wextra -c -o main.o main.c

strcut.o : strcut.c
	gcc -std=c11 -pedantic -Wall -Wextra -c -o strcut.o strcut.c