BIN_DIR="bin"
[ ! -d "$BIN_DIR" ] && mkdir -p "$BIN_DIR"
make server
make client