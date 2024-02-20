all:basurero
basurero: basurero.c
	gcc -o basurero basurero.c -Wall -g -lm