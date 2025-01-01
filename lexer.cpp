#include "lexer.h"

Lexer::Lexer(const std::string& source) : source(source), position(0), tokenRegex(R"((�����|����|�����������|������|������|���|�����|������|��|��|�����|�������|�����|[=+\-*/<>!]=?|�|���|��|[(){};,]|//.*|/\*.*?\*/|[a-zA-Z_][a-zA-Z0-9_]*|\d+(\.\d+)?|"[^"]*"))") {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    std::smatch match;
    std::string remainingSource = source;

    while (std::regex_search(remainingSource, match, tokenRegex)) {
        std::string tokenValue = match.str(0);
        TokenType tokenType = getTokenType(tokenValue);
        tokens.push_back({ tokenType, tokenValue });
        remainingSource = match.suffix().str();
    }

    return tokens;
}

TokenType Lexer::getTokenType(const std::string& tokenValue) {
    if (tokenValue == "�����" || tokenValue == "����" || tokenValue == "�����������" || tokenValue == "������" ||
        tokenValue == "������" || tokenValue == "���" || tokenValue == "�����" || tokenValue == "������" ||
        tokenValue == "��" || tokenValue == "��" || tokenValue == "�����" || tokenValue == "�������" ||
        tokenValue == "�����") {
        return TokenType::KEYWORD;
    }
    else if (tokenValue == "=" || tokenValue == "+" || tokenValue == "-" || tokenValue == "*" || tokenValue == "/" ||
        tokenValue == "==" || tokenValue == "!=" || tokenValue == ">" || tokenValue == "<" ||
        tokenValue == ">=" || tokenValue == "<=" || tokenValue == "�" || tokenValue == "���" ||
        tokenValue == "��") {
        return TokenType::OPERATOR;
    }
    else if (tokenValue == "(" || tokenValue == ")" || tokenValue == "{" || tokenValue == "}" ||
        tokenValue == ";" || tokenValue == ",") {
        return TokenType::SEPARATOR;
    }
    else if (std::regex_match(tokenValue, std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"))) {
        return TokenType::IDENTIFIER;
    }
    else if (std::regex_match(tokenValue, std::regex(R"(\d+(\.\d+)?)"))) {
        return TokenType::LITERAL;
    }
    else if (std::regex_match(tokenValue, std::regex(R"(//.*)")) ||
        std::regex_match(tokenValue, std::regex(R"(/\*.*?\*/)"))) {
        return TokenType::COMMENT;
    }
    else {
        return TokenType::UNKNOWN;
    }
}
