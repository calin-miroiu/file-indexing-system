CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
SRC = main.c
EXEC = search_index

build: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)_bin
	echo '#!/bin/bash' > $(EXEC)
	echo './$(EXEC)_bin < indexare.in > indexare.out' >> $(EXEC)
	chmod +x $(EXEC)

clean:
	rm -f $(EXEC) $(EXEC)_bin indexare.in indexare.out temp.out *.norm