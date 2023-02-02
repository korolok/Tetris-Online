BIN_DIR="bin"
[ ! -d "$BIN_DIR" ] && mkdir -p "$BIN_DIR"
touch bin/config.cfg
echo "7070" > bin/config.cfg
make server >> build.log
make client >> build.log