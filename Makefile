CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = 

# Platform detection
ifdef WSL_DISTRO_NAME
	PLATFORM = wsl
	RM = rm -f
	MKDIR = mkdir -p
	RMDIR = rm -rf
	EXE_EXT =
else ifeq ($(OS),Windows_NT)
	PLATFORM = windows
	CFLAGS += -D_WIN32
	LDFLAGS += -lws2_32
	RM = del /Q /F
	MKDIR = mkdir
	RMDIR = rmdir /S /Q
	EXE_EXT = .exe
else
	PLATFORM = unix
	RM = rm -f
	MKDIR = mkdir -p
	RMDIR = rm -rf
	EXE_EXT =
endif

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/server$(EXE_EXT)

.PHONY: all clean directories

all: directories $(TARGET)

directories:
ifeq ($(PLATFORM),windows)
	@if not exist $(OBJ_DIR) $(MKDIR) $(OBJ_DIR)
	@if not exist $(BIN_DIR) $(MKDIR) $(BIN_DIR)
else
	@$(MKDIR) $(OBJ_DIR) $(BIN_DIR)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RMDIR) $(OBJ_DIR) $(BIN_DIR)

format: *.c
	clang-format -style=Google -i *.c

run:
	./$(TARGET)

test:
	python3 tests/main.py

kill:
ifeq ($(PLATFORM),windows)
	taskkill /F /IM server.exe
else
	sudo kill -9 $(sudo lsof -t -i:8203)
endif

simple:
	docker-compose up --build -d simple

nginx:
	docker-compose up --build -d nginx

python:
	docker-compose up --build -d python

up:
	docker-compose up --build -d

down:
	docker-compose down

simple-cli:
	docker exec -it simple_service /bin/sh
