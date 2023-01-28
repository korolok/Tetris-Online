server: src/server.c src/net.c src/util.c
	gcc src/net.c src/server.c -o bin/server -Wall -Wextra -DDEBUG

client: src/client.c src/net.c src/util.c
	gcc src/net.c src/client.c src/util.c -o bin/client -Wall -Wextra -lpthread -lncurses