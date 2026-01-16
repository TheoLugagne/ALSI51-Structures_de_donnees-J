#include "program/lexical.h"
#include "program/program.h"
#include "program/lexer.h"
#include <string.h>
#include <stdio.h>

bool str_eq(const char* s1, const char *s2, const unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        if (s1[i] == '\0' || s2[i] == '\0' || s1[i] != s2[i])
            return false;
    }
    return true;
}

// Returns true if s (= *p_s) contains the keyword of type keyword_type as a prefix
// If so, fills the token so that it is a token of type keyword
// Moves p_s forward to skip past the keyword
bool process_keyword(const char **p_s, const e_keyword keyword_type, t_prog_token *token) {
    // TODO
    return false; // TODO
}

t_prog_token_list lex(const char *s) {
    t_prog_token_list list = ptl_create_empty_list();

#define NB_KEYWORDS 7
    static const e_keyword keywords[NB_KEYWORDS] = { KW_ASSIGN, KW_IF, KW_ELSE, KW_WHILE, KW_ENDBLOCK, KW_RETURN, KW_PRINT };

    // TODO

    while (*s != '\0') {
        if (*s == ' ' || *s == '\n' || *s == ' ') {

            // TODO

            s++;
            continue;
        }
        else {

            if (0 /* TODO */) {
                t_prog_token token;
                token.token_type = PT_KEYWORD;
                token.content.keyword = KW_ENDBLOCK;
                // TODO
                continue;
            }
        }

        // TODO

        s++;
    }
    
    return list;
}