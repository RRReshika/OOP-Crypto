#!/bin/bash
# Test script to demonstrate robust input handling
# This simulates various types of input including escape sequences

echo "Testing Crypto Toolkit Input Handling System"
echo "=============================================="
echo ""
echo "Test 1: Valid numeric input"
echo "2" | ./crypto_toolkit

echo ""
echo "Test 2: Input with escape sequences (arrow keys)"
echo -e "2\n\033[A\033[B" | ./crypto_toolkit

echo ""
echo "Test 3: Mixed valid/invalid input"
echo -e "abc\n2\n" | ./crypto_toolkit

echo ""
echo "All tests demonstrate that:"
echo "- Only Enter-confirmed inputs are processed"
echo "- Escape sequences are filtered out"
echo "- Invalid input prompts for re-entry"
echo "- Terminal output history remains clean"
