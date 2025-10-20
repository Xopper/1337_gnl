#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_leaks.sh                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ahaloua <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/26 19:30:40 by ahaloua           #+#    #+#              #
#    Updated: 2019/06/26 19:36:06 by ahaloua          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Create test directory
TEST_DIR="leak_tests"
mkdir -p $TEST_DIR

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}  GET_NEXT_LINE Memory Leak Test Suite${NC}"
echo -e "${BLUE}============================================${NC}\n"

# Check if valgrind is installed
if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}ERROR: valgrind is not installed!${NC}"
    echo -e "${YELLOW}Install with: apt-get install valgrind${NC}"
    exit 1
fi

# Clean and compile
echo -e "${YELLOW}[1/5] Cleaning previous builds...${NC}"
make fclean > /dev/null 2>&1

echo -e "${YELLOW}[2/5] Compiling libft...${NC}"
make -C libft > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to compile libft${NC}"
    exit 1
fi

echo -e "${YELLOW}[3/5] Compiling get_next_line...${NC}"
make > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to compile get_next_line${NC}"
    exit 1
fi

echo -e "${GREEN}Compilation successful!${NC}\n"

# Create test files
echo -e "${YELLOW}[4/5] Creating test files...${NC}"

# Test 1: Simple file with newlines
cat > $TEST_DIR/test1.txt << 'EOF'
Line 1
Line 2
Line 3
EOF

# Test 2: File with empty lines
cat > $TEST_DIR/test2.txt << 'EOF'
First line

Third line (second was empty)

Fifth line
EOF

# Test 3: Single line without newline
echo -n "Single line without newline" > $TEST_DIR/test3.txt

# Test 4: Empty file
touch $TEST_DIR/test4.txt

# Test 5: Long lines
python3 -c "print('A' * 1000)" > $TEST_DIR/test5.txt
python3 -c "print('B' * 5000)" >> $TEST_DIR/test5.txt

# Test 6: Many short lines
for i in {1..100}; do
    echo "Line $i" >> $TEST_DIR/test6.txt
done

# Test 7: Only newlines
echo -e "\n\n\n\n" > $TEST_DIR/test7.txt

# Test 8: Mixed content
cat > $TEST_DIR/test8.txt << 'EOF'
Normal line
Line with    tabs		and   spaces

Another line after empty
EOF

echo -e "${GREEN}Test files created!${NC}\n"

# Function to run valgrind test
run_valgrind_test() {
    local test_name=$1
    local test_file=$2

    TESTS_RUN=$((TESTS_RUN + 1))

    echo -e "${BLUE}Test $TESTS_RUN: $test_name${NC}"

    # Run valgrind
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --error-exitcode=42 \
             --log-file=$TEST_DIR/valgrind_$TESTS_RUN.log \
             ./gnl_test "$test_file" > $TEST_DIR/output_$TESTS_RUN.txt 2>&1

    local exit_code=$?

    # Check for memory leaks
    if [ $exit_code -eq 42 ]; then
        echo -e "${RED}  ✗ FAILED - Memory leaks detected!${NC}"
        echo -e "${YELLOW}  Check: $TEST_DIR/valgrind_$TESTS_RUN.log${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    else
        # Check valgrind log for "no leaks are possible"
        if grep -q "All heap blocks were freed -- no leaks are possible" "$TEST_DIR/valgrind_$TESTS_RUN.log"; then
            echo -e "${GREEN}  ✓ PASSED - No memory leaks!${NC}"
            TESTS_PASSED=$((TESTS_PASSED + 1))
            return 0
        elif grep -q "definitely lost: 0 bytes in 0 blocks" "$TEST_DIR/valgrind_$TESTS_RUN.log" && \
             grep -q "indirectly lost: 0 bytes in 0 blocks" "$TEST_DIR/valgrind_$TESTS_RUN.log"; then
            echo -e "${GREEN}  ✓ PASSED - No memory leaks!${NC}"
            TESTS_PASSED=$((TESTS_PASSED + 1))
            return 0
        else
            echo -e "${RED}  ✗ FAILED - Possible memory issues${NC}"
            echo -e "${YELLOW}  Check: $TEST_DIR/valgrind_$TESTS_RUN.log${NC}"
            TESTS_FAILED=$((TESTS_FAILED + 1))
            return 1
        fi
    fi
}

echo -e "${YELLOW}[5/5] Running memory leak tests...${NC}\n"

# Run all tests
run_valgrind_test "Simple file with newlines" "$TEST_DIR/test1.txt"
run_valgrind_test "File with empty lines" "$TEST_DIR/test2.txt"
run_valgrind_test "Single line without newline" "$TEST_DIR/test3.txt"
run_valgrind_test "Empty file" "$TEST_DIR/test4.txt"
run_valgrind_test "Long lines (1000+ chars)" "$TEST_DIR/test5.txt"
run_valgrind_test "Many short lines (100+)" "$TEST_DIR/test6.txt"
run_valgrind_test "Only newlines" "$TEST_DIR/test7.txt"
run_valgrind_test "Mixed content" "$TEST_DIR/test8.txt"

# Print summary
echo -e "\n${BLUE}============================================${NC}"
echo -e "${BLUE}              Test Summary${NC}"
echo -e "${BLUE}============================================${NC}"
echo -e "Total tests run:    $TESTS_RUN"
echo -e "${GREEN}Tests passed:       $TESTS_PASSED${NC}"
if [ $TESTS_FAILED -gt 0 ]; then
    echo -e "${RED}Tests failed:       $TESTS_FAILED${NC}"
else
    echo -e "${GREEN}Tests failed:       $TESTS_FAILED${NC}"
fi
echo -e "${BLUE}============================================${NC}\n"

# Detailed leak summary
if [ $TESTS_FAILED -gt 0 ]; then
    echo -e "${YELLOW}Checking for leak details...${NC}\n"
    for log in $TEST_DIR/valgrind_*.log; do
        if grep -q "definitely lost:" "$log" && ! grep -q "definitely lost: 0 bytes" "$log"; then
            echo -e "${RED}Leak found in $log:${NC}"
            grep -A 5 "definitely lost:" "$log"
            echo ""
        fi
    done
fi

# Final verdict
if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║   ALL TESTS PASSED - NO LEAKS! 🎉     ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════╝${NC}\n"
    exit 0
else
    echo -e "${RED}╔════════════════════════════════════════╗${NC}"
    echo -e "${RED}║   SOME TESTS FAILED - CHECK LOGS ⚠️    ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════╝${NC}\n"
    echo -e "${YELLOW}View detailed logs in: $TEST_DIR/${NC}\n"
    exit 1
fi
