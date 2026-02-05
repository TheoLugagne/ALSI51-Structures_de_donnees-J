#include "program/lexical.h"
#include "program/program.h"
#include "program/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    const char *s = *p_s;
    const char *kw = NULL;
    unsigned int len = 0;

    switch (keyword_type) {
        case KW_ASSIGN:     kw = "=";           len = 1;    break;
        case KW_IF:         kw = "if";          len = 2;    break;
        case KW_FOR:        kw = "for";         len = 3;    break;
        case KW_ELSE:       kw = "else";        len = 4;    break;
        case KW_WHILE:      kw = "while";       len = 5;    break;
        case KW_ENDBLOCK:   kw = "(end-block)"; len = 11;   break;
        case KW_RETURN:     kw = "return";      len = 6;    break;
        case KW_PRINT:      kw = "print";       len = 5;    break;
        default: fprintf(stderr, "process_keyword: Unrecognized keywork");
    }

    if (str_eq(s, kw, len)) {
        token->token_type = PT_KEYWORD;
        token->content.keyword = keyword_type;
        *p_s = s + len;
        return true;
    }
    return false;
}

bool is_kw_await_expr(const e_keyword keyword_type) {
    switch (keyword_type) {
        case KW_ASSIGN:
        case KW_IF:
        case KW_FOR:
        case KW_WHILE:
        case KW_PRINT:
        case KW_RETURN:   return true;
        case KW_ENDBLOCK:
        case KW_ELSE: return false;
        default: fprintf(stderr, "process_keyword: Unrecognized keywork"); return false;
    }
}

bool is_kw_await_endblock(const e_keyword keyword_type) {
    switch (keyword_type) {
        case KW_IF:
        case KW_FOR:
        case KW_WHILE: return true;
        case KW_ELSE:
        case KW_ASSIGN:
        case KW_PRINT:
        case KW_RETURN:
        case KW_ENDBLOCK:   return false;
        default: fprintf(stderr, "process_keyword: Unrecognized keywork"); return false;
    }
}

bool is_allowed_var(const char var) {
    return var >= 'a' && var <= 'z';
}

bool process_var(const char **p_s, t_prog_token *token) {
    const char *s = *p_s;
    if (!is_allowed_var(s[0])) return false;
    token->token_type = PT_VAR;
    token->content.var = s[0];
    (*p_s)++;
    return true;
}

char** substring(const char *str, const int length) {
    char **result = (char **)malloc(sizeof(char*));
    char *sub = (char *)malloc(length + 1);
    memcpy(sub, str, length);
    sub[length] = '\0';
    *result = sub;
    return result;
}

bool process_expr(const char **p_s, t_prog_token *token, bool in_for) {
    int len = 0;
    const char* s = *p_s;

    if (s[len] == '\"') { // string expr
        len++;
        while (s[len] != '\n' && s[len] != '\0' && s[len] != '\"') len++;
        if (len == 0) return false;
        token->token_type = PT_STRING;
        const t_expr expr = parse_expr(p_s);
        token->content.expr = expr;
    } else {
        while (s[len] != '\n' && s[len] != '\0' && s[len] != '\"' && s[len] != ';') len++;
        if (len == 0) return false;
        token->token_type = PT_EXPR;
        t_expr expression;
        if (in_for) {
            while (s[len] != ';' && s[len] != ')') len--;
            const char **sub = substring(s, len);
            expression = parse_expr(sub);
            free(sub);
            *p_s = *p_s + len;
        } else {
            expression = parse_expr(p_s); // parse and move p_s forward
        }
        token->content.expr_rpn = shunting_yard(&expression);
        // precomputing
        simplify_constant_subexpressions_rpn(&token->content.expr_rpn);
        if (is_constant_expr_rpn(&token->content.expr_rpn)) {
            precompute_constant_expr_rpn(&token->content.expr_rpn);
        }
    }
    return true;
}

t_prog_token_list lex(const char *s) {
    t_prog_token_list list = ptl_create_empty_list();

    #define BASE_INDENT 4
    #define NB_KEYWORDS 8
    static constexpr e_keyword keywords[NB_KEYWORDS] = { KW_ASSIGN, KW_IF, KW_ELSE, KW_WHILE, KW_ENDBLOCK, KW_RETURN, KW_PRINT, KW_FOR};

    bool await_expr = false;
    bool await_endblock = false;
    bool in_indent = true;
    bool in_for = false;
    bool skip_expr = false;
    int nb_endblock_awaited = 0;
    int len_indent = 0;
    int curr_indent = 0;

    while (*s != '\0' && *s != EOF) {
        t_prog_token token;
        if (in_indent && *s != ' ') {
            in_indent = false;
        }
        if (in_indent && *s == ' ') {
            len_indent++;
        }
        if (*s == '\n') {
            in_indent = true;
            curr_indent = len_indent;
            len_indent = 0;
        }
        if (skip_expr && *s == '(') {
            s++;
            continue;
        }
        if (*s == ';') {
            s++;
            await_expr = true;
            skip_expr = false;
            continue;
        }
        if (*s == ' ' || *s == '\n' || *s == '\r') {
            s++;
            continue;
        }
        if (nb_endblock_awaited > 0) {
            await_endblock = true;
        }


        // keyword
        bool skip_endblock = !await_endblock;
        bool is_kw = false;
        bool need_to_add_eb = false;
        for (int i = 0; i < NB_KEYWORDS; i++) {
            if (process_keyword(&s, keywords[i], &token)) {
                await_expr = is_kw_await_expr(keywords[i]);
                if (is_kw_await_endblock(keywords[i])) {
                    need_to_add_eb = true;
                }
                skip_endblock = !await_endblock;
                if (keywords[i] == KW_ELSE) {
                    skip_endblock = true;
                }
                if (keywords[i] == KW_FOR) {
                    in_for = true;
                    skip_expr = true;
                }
                is_kw = true;
                break;
            }
        }
        if (len_indent < curr_indent) {
            if (!skip_endblock) {
                const int nb_endblock_to_add = nb_endblock_awaited - len_indent/BASE_INDENT;
                for (int j = 0; j < nb_endblock_to_add; j++) {
                    t_prog_token eb_token;
                    eb_token.token_type = PT_KEYWORD;
                    eb_token.content.keyword = KW_ENDBLOCK;
                    ptl_push_back(&list, eb_token);
                    nb_endblock_awaited--;
                    curr_indent -= BASE_INDENT;
                }
                if (nb_endblock_awaited == 0) {
                    await_endblock = false;
                }
            }
        }
        if (need_to_add_eb) nb_endblock_awaited++;
        if (is_kw) { ptl_push_back(&list, token); continue; }

        if (await_expr && !skip_expr) {
            if (process_expr(&s, &token, in_for)) {
                ptl_push_back(&list, token);
                await_expr = false;
                // Skip to the end of the line, avoid unexpected tokens at the of the program
                while (*s != '\n' && *s != '\0' && *s != ';') {
                    if (*s == ')') {
                        in_for = false;
                    }
                    s++;
                }
                continue;
            }
            fprintf(stderr, "Lexer error: expected expression\n");
            exit(EXIT_FAILURE);
        }
        if (process_var(&s, &token)) {
            ptl_push_back(&list, token);
            continue;
        }

        // Skip unknown characters
        s++;
    }
    
    return list;
}
