# Menu Lifecycle - Explicit Termination Documentation

## Control Flow Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    APPLICATION LOOP                         │
│                     while (true)                            │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  MENU INVOCATION                            │
│              int choice = rMenu(...)                        │
│                                                             │
│  ┌────────────────────────────────────────────────────┐    │
│  │         MENU LOOP STARTS                           │    │
│  │         while (true)                               │    │
│  │                                                     │    │
│  │  ┌──────────────────────────────────────────────┐  │    │
│  │  │  1. Clear screen                             │  │    │
│  │  │  2. Draw logo + menu                         │  │    │
│  │  │  3. Highlight current selection              │  │    │
│  │  │  4. Wait for key input                       │  │    │
│  │  └──────────────────────────────────────────────┘  │    │
│  │                    │                               │    │
│  │                    ▼                               │    │
│  │         ┌──────────────────────┐                  │    │
│  │         │   Key pressed?       │                  │    │
│  │         └──────────────────────┘                  │    │
│  │           │         │         │                   │    │
│  │      ┌────┴────┐ ┌──┴───┐ ┌──┴──────┐           │    │
│  │      │  Up ↑   │ │ Down │ │  Enter  │           │    │
│  │      └────┬────┘ └──┬───┘ └──┬──────┘           │    │
│  │           │         │         │                   │    │
│  │           ▼         ▼         ▼                   │    │
│  │     Update sel  Update sel  TERMINATE            │    │
│  │     Continue    Continue    ┌──────────┐         │    │
│  │     loop        loop        │ return   │         │    │
│  │     │           │           │ rSel;    │         │    │
│  │     └───────┬───┘           └────┬─────┘         │    │
│  │             │                    │               │    │
│  │             └────────►LOOP       │               │    │
│  │                       CONTINUES  │               │    │
│  │                                  │               │    │
│  │                                  ▼               │    │
│  │                    ╔═══════════════════════════╗ │    │
│  │                    ║  LOOP TERMINATED          ║ │    │
│  │                    ║  Menu rendering STOPS     ║ │    │
│  │                    ║  Control exits function   ║ │    │
│  │                    ╚═══════════════════════════╝ │    │
│  └─────────────────────────────────────────────────┘    │
│                                                          │
│  Return value: Selected option index                    │
└──────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│              MENU TERMINATED - EXECUTION BEGINS             │
│                                                             │
│  Menu loop is NO LONGER RUNNING                            │
│  Screen is clear                                           │
│  Ready for execution output                                │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  EXECUTE SELECTED OPTION                    │
│              switch (choice) { ... }                        │
│                                                             │
│  Menu will NOT re-render during execution                  │
│  Only selected action's output appears                     │
│  Non-selected options never displayed                      │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                   ACTION COMPLETES                          │
│             User presses Enter to continue                  │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│              APPLICATION LOOP CONTINUES                     │
│          Menu is RE-INVOKED (fresh menu loop)               │
└─────────────────────────────────────────────────────────────┘
```

## Code Lifecycle Breakdown

### Phase 1: Menu Invocation
```cpp
// In rInit() - Application loop
int rCh = rUI::rMenu("MAIN MENU", rOpts, rInfo);
                      ↑
                      └─── Control enters menu function
```

### Phase 2: Menu Loop Active
```cpp
// In rMenu() - Dedicated navigation loop
while (true) {
    // Clear and redraw
    std::cout << "\033[2J\033[1;1H";
    // ... render menu ...
    
    int rKey = rGetKey();
    
    if (rKey == 1000) {           // Up arrow
        rSel = (rSel - 1 + N) % N;
        // LOOP CONTINUES ──┐
    }                           │
    else if (rKey == 1001) {    │     // Down arrow
        rSel = (rSel + 1) % N;  │
        // LOOP CONTINUES ──┤
    }                           │
    else if (rKey == 10) {      │     // Enter
        std::cout << "\033[2J\033[1;1H";
        return rSel;  ◄─────────┘     // LOOP TERMINATES
    }                                 // Control exits function
    // Loop iteration ends here ──┘
}
```

### Phase 3: Menu Terminated
```cpp
// Back in rInit() - Menu function returned
int rCh = rUI::rMenu("MAIN MENU", rOpts, rInfo);
          ↑         └─── Control returned here
          │              Menu loop is NO LONGER RUNNING
          └── rCh contains selected index
```

### Phase 4: Execution
```cpp
// Execute selected action
if (rCh == 0) executeOption0();
else if (rCh == 1) executeOption1();
// ...

// Menu does NOT re-render during execution
// Only action's output appears
```

## Key Guarantees

### ✅ Loop Termination
```cpp
return rSel;  // Immediate exit, no fall-through
              // No continue statement after Enter
              // No conditions that could skip return
```

### ✅ No Implicit Re-invocation
```cpp
// Menu function returns to caller
// Caller must explicitly call rMenu() again
// Menu does NOT call itself
// Menu does NOT remain in memory loop
```

### ✅ Single Active Loop Instance
```cpp
// Only ONE menu loop exists at a time
// Previous loop MUST terminate before new one starts
// No nested menu loops
// No concurrent menu rendering
```

## Verification Checklist

After pressing Enter on a menu option:

- [ ] Menu loop terminates (function returns)
- [ ] Screen clears completely
- [ ] Menu does NOT redraw
- [ ] Only selected action executes
- [ ] Non-selected options never appear
- [ ] Menu only reappears when explicitly re-invoked

## Common Anti-Patterns AVOIDED

### ❌ Loop Without Termination
```cpp
// WRONG - Never exits
while (true) {
    if (enter) {
        execute();
        // Missing return/break - loop continues!
    }
}
```

### ❌ Menu Refresh After Selection
```cpp
// WRONG - Menu redraws after action
if (enter) {
    execute();
    drawMenu();  // Don't do this!
}
```

### ❌ Conditional Return
```cpp
// WRONG - May not exit
if (enter && someCondition) {
    return;  // Only returns if condition true
}
```

## Correct Implementation ✓

### ✅ Unconditional Immediate Return
```cpp
// CORRECT - Always exits on Enter
else if (rKey == 10 || rKey == 13) {
    std::cout << "\033[2J\033[1;1H";
    return rSel;  // Guaranteed exit
}
```

### ✅ Clear Before Return
```cpp
// CORRECT - Clean state for execution
std::cout << "\033[2J\033[1;1H";  // Clear menu
std::cout.flush();                // Ensure output
return rSel;                      // Then exit
```

### ✅ Explicit Re-invocation
```cpp
// CORRECT - Menu called only when needed
while (true) {           // Application loop
    int ch = rMenu();    // Menu invocation
    execute(ch);         // Execution
}                        // Loop for next menu
```

## Summary

The menu implements **explicit lifecycle termination**:

1. **Dedicated Loop**: Menu runs in isolated `while(true)` loop
2. **Arrow Navigation**: Updates selection, continues loop
3. **Enter Press**: Executes `return rSel` - immediate termination
4. **Control Transfer**: Returns to caller, execution begins
5. **No Re-rendering**: Menu only reappears when explicitly re-invoked

This ensures:
- Clean separation between navigation and execution
- No menu pollution in output history
- No unintended display of non-selected options
- Predictable, modal behavior
