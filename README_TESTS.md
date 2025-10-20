# Get Next Line - Memory Leak Testing

## Overview

This repository includes a comprehensive memory leak testing suite for the `get_next_line` project using Valgrind.

## Files

- **test_leaks.sh**: Automated memory leak testing script
- **Makefile**: Builds the project with debugging symbols
- **MEMORY_LEAK_ANALYSIS.md**: Detailed memory management analysis

## Requirements

### Install Valgrind

```bash
# Ubuntu/Debian
sudo apt-get install valgrind

# macOS
brew install valgrind

# Check installation
valgrind --version
```

## Quick Start

### Run All Tests

```bash
./test_leaks.sh
```

This will:
1. Clean previous builds
2. Compile libft
3. Compile get_next_line with debug flags
4. Create 8 different test scenarios
5. Run valgrind on each test
6. Display results with color-coded output

## Test Scenarios

The script tests the following scenarios:

| Test | Description | Purpose |
|------|-------------|---------|
| 1 | Simple file with newlines | Basic functionality |
| 2 | File with empty lines | Empty line handling |
| 3 | Single line without newline | EOF without \\n |
| 4 | Empty file | Edge case |
| 5 | Long lines (1000+ chars) | Large buffer handling |
| 6 | Many short lines (100+) | Multiple allocations |
| 7 | Only newlines | Empty content lines |
| 8 | Mixed content | Real-world scenario |

## Understanding Results

### Success Output

```
✓ PASSED - No memory leaks!
```

All memory was properly allocated and freed.

### Failure Output

```
✗ FAILED - Memory leaks detected!
Check: leak_tests/valgrind_X.log
```

Memory leaks were found. Check the log file for details.

## Detailed Logs

All test results are saved in the `leak_tests/` directory:

- **valgrind_X.log**: Detailed Valgrind output for test X
- **output_X.txt**: Program output for test X
- **testX.txt**: Input file used for test X

### Reading Valgrind Logs

```bash
cat leak_tests/valgrind_1.log
```

Key sections:
- **HEAP SUMMARY**: Total allocations vs frees
- **LEAK SUMMARY**: Types and amounts of leaks
- **Stack traces**: Where leaks originated

## Manual Testing

### Single File Test

```bash
# Compile
make

# Run with valgrind
valgrind --leak-check=full ./gnl_test your_file.txt
```

### Check Specific Scenario

```bash
# Create a test file
echo -e "Line 1\nLine 2\nLine 3" > test.txt

# Run with valgrind
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./gnl_test test.txt
```

## Common Leak Types

### Definitely Lost
Memory that was allocated but never freed and no pointers remain.

**Fix**: Ensure `free()` is called for every `malloc()`.

### Indirectly Lost
Memory that was lost because the pointer to it was lost.

**Fix**: Free all child allocations before freeing parent structures.

### Still Reachable
Memory still pointed to at exit but not freed.

**Note**: Usually not a problem for short programs but should be freed in production.

## Expected Behavior

With correct implementation:
- **All tests should PASS** ✓
- **0 bytes leaked** in all scenarios
- **All heap blocks freed** at exit

## Implementation Notes

### Caller Responsibilities

When using `get_next_line()`:

```c
int fd = open("file.txt", O_RDONLY);
char *line;
int ret;

while ((ret = get_next_line(fd, &line)) > 0)
{
    // Use line
    free(line);  // MUST free each line
}

if (ret == 0)
    free(line);  // Free last line (EOF case)

close(fd);
```

### Why Free on ret == 0?

When `get_next_line()` returns 0:
- It means EOF was reached
- But the last line (if any) is still allocated
- You must free it even though ret == 0

### Error Handling (ret == -1)

When `get_next_line()` returns -1:
- An error occurred
- No line was allocated
- Don't free (nothing to free)

## Clean Up

```bash
# Remove test files and logs
rm -rf leak_tests/

# Clean compiled files
make fclean
```

## Troubleshooting

### "valgrind: command not found"

Install Valgrind (see Requirements section).

### "Compilation failed"

Ensure all source files are present:
- get_next_line.c
- get_next_line.h
- main.c
- libft/ directory with all sources

### Tests failing with leaks

1. Check the specific valgrind log file
2. Look for the stack trace showing where the leak occurred
3. Ensure proper free() calls in error paths
4. Verify return value handling in main.c

### Permission denied

```bash
chmod +x test_leaks.sh
```

## Advanced Usage

### Custom Buffer Size

Edit `get_next_line.h`:

```c
#define BUFF_SIZE 32  // Change this value
```

Then recompile and retest:

```bash
make re
./test_leaks.sh
```

### Add Custom Tests

Edit `test_leaks.sh` and add:

```bash
# Create custom test file
cat > $TEST_DIR/test9.txt << 'EOF'
Your custom content here
EOF

# Run the test
run_valgrind_test "Custom test description" "$TEST_DIR/test9.txt"
```

## Continuous Integration

Add to your CI pipeline:

```bash
#!/bin/bash
./test_leaks.sh
if [ $? -eq 0 ]; then
    echo "Memory tests passed!"
    exit 0
else
    echo "Memory tests failed!"
    exit 1
fi
```

## Performance Notes

- Valgrind slows down execution significantly (10-50x)
- This is normal and expected
- Tests may take longer with large files
- Timeout is set to 2 minutes per test

## Support

For issues or questions:
1. Check MEMORY_LEAK_ANALYSIS.md for implementation details
2. Review valgrind log files for specific errors
3. Ensure main.c properly handles return values

## License

This testing suite is provided as-is for educational purposes.

## Credits

- Valgrind by Julian Seward and contributors
- 42 School get_next_line project
