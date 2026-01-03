# Crypto Toolkit - Robust Input Handling System

## Implementation Summary

### Key Features Implemented

1. **Enter-Only Input Processing**
   - All inputs require Enter key confirmation
   - No raw character-by-character reading
   - Standard line buffering ensures clean input handling

2. **Escape Sequence Filtering**
   - `rSanitize()` function removes all terminal escape sequences
   - Filters arrow keys, cursor movements, and scroll inputs
   - Only printable characters (ASCII 32-126) are accepted

3. **Robust Validation System**
   - `rGetInput(min, max)` - Range-validated numeric input
   - `rGetOpt()` - Sanitized optional numeric input
   - Whitespace trimming and input normalization
   - Digit-only validation before parsing

4. **Clean Terminal Interface**
   - No screen clearing (removes `\033[2J\033[1;1H`)
   - Output history remains visible
   - Numbered menu options (1, 2, 3...) instead of arrow navigation
   - Clear error messages for invalid input

### Technical Changes

#### Before (Arrow-Key System)
```cpp
// Used raw terminal mode (termios)
// Captured individual keystrokes
// Cleared screen on every menu redraw
// Required escape sequence handling for arrows
```

#### After (Robust Input System)
```cpp
// Uses standard line-buffered input (getline)
// Only processes complete lines (Enter-confirmed)
// Preserves terminal history
// Filters all escape sequences automatically
```

### Input Sanitization Algorithm

```cpp
inline std::string rSanitize(const std::string& rIn) {
    // 1. Detect escape sequence start (ESC or \033)
    // 2. Skip all characters until sequence terminator
    // 3. Only keep printable ASCII characters
    // 4. Return cleaned string
}
```

### Validation Flow

```
User Input → getline() → rSanitize() → Trim Whitespace → 
Digit Validation → Range Check → Return Valid Integer
       ↓ (if invalid at any step)
   Error Message → Retry Input
```

### Benefits

- ✅ No escape sequences appear in output
- ✅ Terminal scrollback preserved
- ✅ Robust against malformed input
- ✅ Clear user feedback on errors
- ✅ Portable (no platform-specific terminal control)
- ✅ Maintains university coursework standards

### Files Modified

1. **src/RUIUtils.h**
   - Removed termios dependency
   - Removed `rGetCh()` raw input function
   - Added `rSanitize()` for escape sequence filtering
   - Added `rGetInput()` for validated numeric input
   - Redesigned `rMenu()` for numbered selection

2. **src/RMenu.cpp**
   - Updated `rGetOpt()` with sanitization and validation
   - All menu interactions now use Enter-confirmed input

### Usage Examples

**Menu Selection:**
```
+--------------------------------------+
| CRYPTO TOOLKIT - WELCOME             |
+--------------------------------------+
| 1. Register                          |
| 2. Login                             |
| 3. Exit                              |
+--------------------------------------+

Enter choice (1-3): 2
```

**Invalid Input Handling:**
```
Enter choice (1-3): abc
[✗] Invalid input. Enter a number (1-3): 5
[✗] Out of range. Enter a number (1-3): 2
```

**Escape Sequences Filtered:**
```
Enter choice (1-3): ^[[A^[[B2  ← Arrow keys ignored, only "2" processed
```

## Testing

Run the application and test:
1. Valid numeric input
2. Invalid input (letters, symbols)
3. Out-of-range numbers
4. Arrow key presses (should be ignored)
5. Mixed input with escape sequences

All scenarios are handled gracefully with appropriate error messages.
