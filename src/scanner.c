#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct
{
    const char *start;
    const char *current;
    int line;
} Scanner;
Scanner scanner;

bool isAtEnd()
{
    return *scanner.current == '\0';
}

void initScanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

bool match(char expected)
{
    if (isAtEnd())
        return false;
    if (*scanner.current != expected)
        return false;
    scanner.current++;
    return true;
}

char advance()
{
    scanner.current++;

    return scanner.current[-1];
}

Token makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static char peek()
{
    return *scanner.current;
}

static char peekNext()
{
    if (isAtEnd())
        return '\0';
    return scanner.current[1];
}

static void skipWhitespace() {
  for (;;) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        scanner.line++;
        advance();
        break;
      case '/':
        if (peekNext() == '/') {
          while (peek() != '\n' && !isAtEnd()) advance();
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static Token errorToken(const char *message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

static Token string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            scanner.line++;
        advance();
    }
    if (isAtEnd())
        return errorToken("Unterminated string.");
    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static Token number()
{
    while (isDigit(peek()))
        advance();
    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext()))
    {
        // Consume the ".".
        advance();
        while (isDigit(peek()))
            advance();
    }
    return makeToken(TOKEN_NUMBER);
}

static bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

static TokenType checkKeyword(int start, int length,
                              const char *rest, TokenType type)
{
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0)
    {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType()
{
    switch (scanner.start[0])
    {
    case 'e':
        return checkKeyword(1, 1, "n", TOKEN_AND);
    case 'k':
        return checkKeyword(1, 3, "las", TOKEN_CLASS);
    case 'a':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'n':
                return checkKeyword(2, 6, "derste", TOKEN_ELSE);
            case 's':
                return TOKEN_IF;
            }
        }
        break;
    case 'n':
        return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o':
        return checkKeyword(1, 1, "f", TOKEN_OR);
    case 'v':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'i':
                return checkKeyword(2, 1, "r", TOKEN_FOR);
            case 'e':
                return checkKeyword(2, 5, "rtoon", TOKEN_PRINT);
            }
        }
        break;
    case 's':
        return checkKeyword(1, 4, "uper", TOKEN_SUPER);
    case 'l':
        return checkKeyword(1, 2, "at", TOKEN_VAR);
    case 't':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'e':
                if (scanner.current - scanner.start > 2)
                {
                    switch (scanner.start[2])
                    {
                    case 'r':
                        if (scanner.current - scanner.start > 3)
                        {
                            switch (scanner.start[3])
                            {
                            case 'w':
                                return checkKeyword(4, 2, "yl", TOKEN_WHILE);
                            case 'u':
                                return checkKeyword(4, 1, "g", TOKEN_RETURN);
                            }
                        }
                    }
                }
            }
        }
        break;
    case 'w':
        return checkKeyword(1, 3, "aar", TOKEN_TRUE);
    case 'd':
        return checkKeyword(1, 2, "it", TOKEN_THIS);
    case 'f':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'a':
                return checkKeyword(2, 2, "ls", TOKEN_FALSE);
            case 'u':
                return checkKeyword(2, 1, "n", TOKEN_FUN);
            }
        }
        break;
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    return makeToken(identifierType());
}

Token scanToken()
{
    skipWhitespace();
    scanner.start = scanner.current;
    if (isAtEnd())
        return makeToken(TOKEN_EOF);
    char c = advance();
#ifdef DEBUG_PRINT_CODE
    printf("AFIRLANG INFO : Character %c\n", c);
#endif
    if (isDigit(c))
        return number();
    if (isAlpha(c))
        return identifier();
    switch (c)
    {
    case '(':
        return makeToken(TOKEN_LEFT_PAREN);
    case ')':
        return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
        return makeToken(TOKEN_LEFT_BRACE);
    case '}':
        return makeToken(TOKEN_RIGHT_BRACE);
    case ';':
        return makeToken(TOKEN_SEMICOLON);
    case ',':
        return makeToken(TOKEN_COMMA);
    case '.':
        return makeToken(TOKEN_DOT);
    case '-':
        return makeToken(TOKEN_MINUS);
    case '+':
        return makeToken(TOKEN_PLUS);
    case '/':
        return makeToken(TOKEN_SLASH);
    case '*':
        return makeToken(TOKEN_STAR);
    case '!':
        return makeToken(
            match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
        return makeToken(
            match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
        return makeToken(
            match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
        return makeToken(
            match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"':
        return string();
    }
    return errorToken("Unexpected character.");
}