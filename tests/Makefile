CC = gcc
CFLAGS = -I../include -Iunity
LDFLAGS = -lm

# Unity test framework files
UNITY_SRC = unity/unity.c
UNITY_HEADER = unity/unity.h unity/unity_internals.h

# Test source files
TEST_SRC = test_http_request.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# Main test executable
TEST_EXE = test_runner

.PHONY: all clean run

all: $(TEST_EXE)

$(TEST_EXE): $(TEST_OBJ) $(UNITY_SRC:.c=.o)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(UNITY_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f $(TEST_OBJ) $(UNITY_SRC:.c=.o) $(TEST_EXE) 