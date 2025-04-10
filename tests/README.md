# C Unit Tests

This directory contains unit tests for the C components of the project using the Unity test framework.

## Setup

1. First, download the Unity test framework:
```bash
./setup_unity.sh
```

2. Build the tests:
```bash
make
```

3. Run the tests:
```bash
make run
```

## Adding New Tests

To add a new test file:

1. Create a new `.c` file in this directory
2. Include the Unity headers and the source file you want to test
3. Add test functions using the Unity test macros
4. Add your test file to `TEST_SRC` in the Makefile

Example test structure:
```c
#include "unity/unity.h"
#include "../src/your_source_file.c"

void test_your_function(void) {
    // Setup
    // Test assertions
    // Cleanup
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_your_function);
    return UNITY_END();
}
```

## Test Organization

- `test_http_request.c`: Tests for HTTP request parsing
- More test files can be added for other components

## Integration Tests

Integration tests are written in Python and can be found in the parent directory's `tests` folder. 