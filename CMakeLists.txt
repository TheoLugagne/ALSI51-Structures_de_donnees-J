cmake_minimum_required(VERSION 3.5)
project(compiler_proj C)

set(CMAKE_C_STANDARD 23)

include_directories(include/)

add_executable(compiler_proj
        src/main.c
        src/structures/list_double-ended.c
        src/structures/prog_token_list.c
        src/structures/queue.c
        src/structures/stack.c
        src/program/lexer.c
        src/program/lexical.c
        src/program/parser.c
        src/program/program.c
        src/program/run.c
        src/file_io/file.c
        src/expressions/expr.c
        src/expressions/operator.c
        src/expressions/expr_token.c
)