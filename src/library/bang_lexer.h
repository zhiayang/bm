#ifndef BANG_LEXER_H_
#define BANG_LEXER_H_

#include <stdlib.h>

#include "./sv.h"

typedef enum {
    BANG_TOKEN_KIND_NAME,
    BANG_TOKEN_KIND_OPEN_PAREN,
    BANG_TOKEN_KIND_CLOSE_PAREN,
    BANG_TOKEN_KIND_OPEN_CURLY,
    BANG_TOKEN_KIND_CLOSE_CURLY,
    BANG_TOKEN_KIND_SEMICOLON,
    BANG_TOKEN_KIND_LIT_STR,
} Bang_Token_Kind;

const char *bang_token_kind_name(Bang_Token_Kind kind);

typedef struct {
    size_t row;
    size_t col;
    const char *file_path;
} Bang_Loc;

#define Bang_Loc_Fmt "%s:%zu:%zu"
#define Bang_Loc_Arg(loc) (loc).file_path, (loc).row, (loc).col

typedef struct {
    Bang_Token_Kind kind;
    String_View text;
    Bang_Loc loc;
} Bang_Token;

typedef struct {
    String_View content;
    const char *line_start;
    String_View line;
    const char *file_path;
    size_t row;

    Bang_Token peek_buffer;
    bool peek_buffer_full;
} Bang_Lexer;

Bang_Lexer bang_lexer_from_sv(String_View content, const char *file_path);

Bang_Loc bang_lexer_loc(Bang_Lexer *lexer);

Bang_Token bang_lexer_expect_token(Bang_Lexer *lexer, Bang_Token_Kind kind);
Bang_Token bang_lexer_expect_keyword(Bang_Lexer *lexer, String_View name);
bool bang_lexer_next(Bang_Lexer *lexer, Bang_Token *token);
bool bang_lexer_peek(Bang_Lexer *lexer, Bang_Token *token);

#endif // BANG_LEXER_H_
