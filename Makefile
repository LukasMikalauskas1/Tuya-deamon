INC_DIR = inc
LIB_DIR = lib
SRC_DIR = src

LIB_NAMES := -llink_core -lmiddleware_implementation -lplatform_port -lutils_modules

CFLAGS = -Wall -Werror
LDFLAGS = -I$(INC_DIR) -L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR) $(LIB_NAMES) 

BIN = tuyad

CFILES = $(wildcard $(SRC_DIR)/*.c)

all:$(BIN)

$(BIN):
	$(CC) $(CFLAGS) -o $@ $(CFILES) $(LDFLAGS)
clean:
	$(RM) -rf $(BIN) $(BUILD_DIR)