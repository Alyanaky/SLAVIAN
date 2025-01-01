#include "lexer.h"

Lexer::Lexer(const std::string& source) : source(source), position(0), tokenRegex(R"((даждь|цело|вещественно|строка|истина|аще|иначе|доколе|от|до|твори|воздать|введи|[=+\-*/<>!]=?|и|или|не|[(){};,]|//.*|/\*.*?\*/|[a-zA-Z_][a-zA-Z0-9_]*|\d+(\.\d+)?|"[^"]*"))") {}

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
    if (tokenValue == "даждь" || tokenValue == "цело" || tokenValue == "вещественно" || tokenValue == "строка" ||
        tokenValue == "истина" || tokenValue == "аще" || tokenValue == "иначе" || tokenValue == "доколе" ||
        tokenValue == "от" || tokenValue == "до" || tokenValue == "твори" || tokenValue == "воздать" ||
        tokenValue == "введи") {
        return TokenType::KEYWORD;
    }
    else if (tokenValue == "=" || tokenValue == "+" || tokenValue == "-" || tokenValue == "*" || tokenValue == "/" ||
        tokenValue == "==" || tokenValue == "!=" || tokenValue == ">" || tokenValue == "<" ||
        tokenValue == ">=" || tokenValue == "<=" || tokenValue == "и" || tokenValue == "или" ||
        tokenValue == "не") {
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
