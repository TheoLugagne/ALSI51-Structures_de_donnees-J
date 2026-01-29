# Step-by-Step Debugging Guide

## Installation

If GDB is not installed:
```bash
sudo apt update
sudo apt install -y gdb
```

## Quick Start

### Option 1: Use the debug script
```bash
./debug.sh
```

### Option 2: Manual GDB session
```bash
# Rebuild with debug symbols
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Start GDB
gdb ./compiler_proj
```

## GDB Commands for Step-by-Step Execution

### Basic Navigation
- `run` or `r` - Start program execution
- `step` or `s` - Step **into** function calls (goes inside functions)
- `next` or `n` - Step **over** function calls (executes function without entering)
- `continue` or `c` - Continue execution until next breakpoint
- `finish` or `fin` - Execute until current function returns
- `quit` or `q` - Exit GDB

### Breakpoints
- `break main` - Set breakpoint at main()
- `break run_program` - Set breakpoint at run_program()
- `break run` - Set breakpoint at run()
- `break run_aux` - Set breakpoint at run_aux() (recursive execution function)
- `break lex` - Set breakpoint at lexer
- `break parse` - Set breakpoint at parser
- `break <file>:<line>` - Set breakpoint at specific line (e.g., `break main.c:103`)
- `info breakpoints` - List all breakpoints
- `delete <num>` - Delete breakpoint by number
- `disable <num>` - Disable breakpoint temporarily
- `enable <num>` - Re-enable breakpoint

### Inspecting Variables
- `print <variable>` or `p <variable>` - Print variable value
- `print var_value[0]` - Print array element
- `print *prog` - Print structure contents
- `print prog->command` - Print structure member
- `info locals` - Show all local variables
- `info args` - Show function arguments
- `info registers` - Show CPU registers

### Viewing Code
- `list` or `l` - Show source code around current line
- `list <function>` - Show source code of function
- `list <file>:<line>` - Show source code at specific location
- `list +` - Show next lines
- `list -` - Show previous lines

### Execution Flow
- `backtrace` or `bt` - Show call stack
- `frame <num>` - Switch to different stack frame
- `up` - Move up call stack
- `down` - Move down call stack

## Example Debugging Session

```bash
cd build
gdb ./compiler_proj

# In GDB:
(gdb) break main
(gdb) run
(gdb) next                    # Step over read_file()
(gdb) print code              # See the loaded code
(gdb) break run_program
(gdb) continue                # Continue to run_program
(gdb) step                    # Step into lex()
(gdb) next                    # Step over lex operations
(gdb) break run_aux
(gdb) continue                # Continue to execution
(gdb) step                    # Step through AST execution
(gdb) print prog->command     # See current command type
(gdb) print var_value[0]      # See variable 'a' value
(gdb) continue                # Continue execution
```

## Key Functions to Debug

1. **main()** - Entry point, file reading
2. **run_program()** - Main execution flow (lexing → parsing → running)
3. **lex()** - Tokenization of source code
4. **parse()** - AST construction from tokens
5. **run()** - Program execution initialization
6. **run_aux()** - Recursive AST execution (Assignment, Print, If, While, Return)

## Tips

- Use `step` to go deep into function calls
- Use `next` to skip over function internals
- Set breakpoints at function entry points for easier navigation
- Use `print` frequently to inspect variable states
- Use `backtrace` to understand the call hierarchy
