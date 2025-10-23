# === Project settings ===
APP = snake
SRC_DIR = src
INCLUDE_DIR = include
SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/Game.cpp $(SRC_DIR)/Snake.cpp $(SRC_DIR)/Food.cpp $(SRC_DIR)/Global.cpp

# === Compiler settings ===
CC = clang++
CFLAGS = -Wall -std=c++17 -I$(INCLUDE_DIR) $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

# === Default target ===
all: $(APP)

$(APP): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(APP)

# === Run target ===
run: $(APP)
	./$(APP)

# === Clean target ===
clean:
	rm -f $(APP)
