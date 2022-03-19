.PHONY: clean, all
CC = gcc
CFLAGS = -std=c99 -Wall -lm -g

all: tema3
tema3: InOut/IO.c HtmlTree/TSelector.c GenericList/TGL.c InOut/Parse.c HtmlTree/TTree.c
	$(CC) $^ -o $@
run:
	./tema3 ./in/input.html ./in/commands.in ./out/commands.out
clean:
	rm -rf tema3 ./out/commands.out ./out/valgrind-out.txt
valgrind: 
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=./out/valgrind-out.txt \
         ./tema3 ./in/input.html ./in/commands.in ./out/commands.out
