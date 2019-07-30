CC = gcc
TEXTS = one_kilo.txt ten_kilo.txt hundred_kilo.txt one_mega.txt ten_mega.txt hundred_mega.txt one_giga.txt ten_giga.txt one_giga_small_change.txt 
EXECS = a.out


test: largefile.c 
	gcc -Wall -Werror -pedantic -std=c11 -O3 largefile.c 

.PHONY: clean
clean: 
	rm -f $(TEXTS) $(EXECS) 
