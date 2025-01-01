#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <regex>

enum class TokenType {
    KEYWORD,
    OPERATOR,
    SEPARATOR,
    IDENTIFIER,
    LITERAL,
    COMMENT,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position;
    std::regex tokenRegex;

    TokenType getTokenType(const std::string& tokenValue);
};

#endif // LEXER_H
