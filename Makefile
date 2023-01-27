server: src/server.c src/net.c src/util.c
	gcc src/net.c src/server.c -o bin/server -Wall -Wextra -DDEBUG