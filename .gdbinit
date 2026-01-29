# GDB initialization file for step-by-step debugging
# This file is automatically loaded when GDB starts

# Set breakpoints at key functions
break main
break run_program
break run
break run_aux
break lex
break parse

# Display source code automatically
set listsize 20

# Print function arguments
set print pretty on

# Show current line
set disassembly-flavor intel

# Useful aliases
define sstep
    step
    list
end

define nnext
    next
    list
end

document sstep
Step into function and show source code
end

document nnext
Step over function and show source code
end
