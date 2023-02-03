server: src/server.c src/net.c src/game.c src/util.c
	@echo "Building Server"
	gcc src/net.c src/server.c src/game.c src/util.c -o bin/tetris_server -Wall -Wextra -DDEBUG

client: src/client.c src/net.c src/util.c
	@echo "Building Client"
	gcc src/net.c src/client.c src/util.c -o bin/tetris_client -Wall -Wextra -lpthread -lncurses