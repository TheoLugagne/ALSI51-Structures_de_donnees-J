#ifndef PARSER_H
#define PARSER_H

#include "program.h"
#include "structures/prog_token_list.h"

t_ast *parse(const t_prog_token_list *token_list);

#endif