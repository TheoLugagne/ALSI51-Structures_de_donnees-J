#!/bin/bash
# Debug script for step-by-step execution

# Rebuild with debug symbols
echo "Building with debug symbols..."
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Run with GDB
echo ""
echo "Starting GDB debugger..."
echo "Useful commands:"
echo "  break main          - Set breakpoint at main()"
echo "  break run_program   - Set breakpoint at run_program()"
echo "  break run           - Set breakpoint at run()"
echo "  break run_aux       - Set breakpoint at run_aux()"
echo "  break lex           - Set breakpoint at lex()"
echo "  break parse         - Set breakpoint at parse()"
echo "  step                - Step into function (s)"
echo "  next                - Step over function (n)"
echo "  continue            - Continue execution (c)"
echo "  print <var>         - Print variable value"
echo "  list                - Show source code"
echo "  quit                - Exit GDB (q)"
echo ""
echo "Press Enter to start debugging..."
read

gdb ./compiler_proj
