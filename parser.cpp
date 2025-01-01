#include "parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

std::shared_ptr<AstNode> Parser::parse() {
    return parseProgram();
}

std::shared_ptr<AstNode> Parser::parseProgram() {
    std::shared_ptr<ProgramNode> program = std::make_shared<ProgramNode>();
    while (position < tokens.size()) {
        program->statements.push_back(parseStatement());
    }
    return program;
}

std::shared_ptr<AstNode> Parser::parseStatement() {
    if (matchKeyword("даждь")) {
        return parseDeclaration();
    }
    else if (matchKeyword("аще")) {
        return parseIfStatement();
    }
    else if (matchKeyword("доколе")) {
        return parseWhileStatement();
    }
    else if (matchKeyword("твори")) {
        return parseFunctionDeclaration();
    }
    else if (matchKeyword("воздать")) {
        return parseReturnStatement();
    }
    else {
        return parseAssignment();
    }
}

std::shared_ptr<AstNode> Parser::parseDeclaration() {
    std::shared_ptr<DeclarationNode> declaration = std::make_shared<DeclarationNode>();
    declaration->identifier = consumeIdentifier();
    declaration->type = consumeType();
    consumeToken("=");
    declaration->expression = parseExpression();
    consumeToken(";");
    return declaration;
}

std::shared_ptr<AstNode> Parser::parseAssignment() {
    std::shared_ptr<AssignmentNode> assignment = std::make_shared<AssignmentNode>();
    assignment->identifier = consumeIdentifier();
    consumeToken("=");
    assignment->expression = parseExpression();
    consumeToken(";");
    return assignment;
}

std::shared_ptr<AstNode> Parser::parseIfStatement() {
    std::shared_ptr<IfStatementNode> ifStatement = std::make_shared<IfStatementNode>();
    consumeToken("аще");
    consumeToken("(");
    ifStatement->condition = parseExpression();
    consumeToken(")");
    consumeToken("{");
    ifStatement->thenStatements = parseStatementList();
    consumeToken("}");
    consumeToken("иначе");
    consumeToken("{");
    ifStatement->elseStatements = parseStatementList();
    consumeToken("}");
    return ifStatement;
}

std::shared_ptr<AstNode> Parser::parseWhileStatement() {
    std::shared_ptr<WhileStatementNode> whileStatement = std::make_shared<WhileStatementNode>();
    consumeToken("доколе");
    consumeToken("(");
    whileStatement->condition = parseExpression();
    consumeToken(")");
    consumeToken("{");
    whileStatement->statements = parseStatementList();
    consumeToken("}");
    return whileStatement;
}

std::shared_ptr<AstNode> Parser::parseFunctionDeclaration() {
    std::shared_ptr<FunctionDeclarationNode> function = std::make_shared<FunctionDeclarationNode>();
    consumeToken("твори");
    function->identifier = consumeIdentifier();
    consumeToken("(");
    function->parameters = parseParameterList();
    consumeToken(")");
    consumeToken("{");
    function->statements = parseStatementList();
    consumeToken("}");
    return function;
}

std::shared_ptr<AstNode> Parser::parseReturnStatement() {
    std::shared_ptr<ReturnStatementNode> returnStatement = std::make_shared<ReturnStatementNode>();
    consumeToken("воздать");
    returnStatement->expression = parseExpression();
    consumeToken(";");
    return returnStatement;
}

std::vector<std::shared_ptr<AstNode>> Parser::parseStatementList() {
    std::vector<std::shared_ptr<AstNode>> statements;
    while (!matchToken("}")) {
        statements.push_back(parseStatement());
    }
    return statements;
}

std::shared_ptr<AstNode> Parser::parseExpression() {
    std::shared_ptr<AstNode> left = parseTerm();
    while (matchToken("+") || matchToken("-")) {
        std::string op = consumeToken();
        std::shared_ptr<AstNode> right = parseTerm();
        left = std::make_shared<ExpressionNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<AstNode> Parser::parseTerm() {
    std::shared_ptr<AstNode> left = parseFactor();
    while (matchToken("*") || matchToken("/")) {
        std::string op = consumeToken();
        std::shared_ptr<AstNode> right = parseFactor();
        left = std::make_shared<TermNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<AstNode> Parser::parseFactor() {
    if (matchToken("(")) {
        consumeToken("(");
        std::shared_ptr<AstNode> expression = parseExpression();
        consumeToken(")");
        return expression;
    }
    else if (matchToken(TokenType::IDENTIFIER)) {
        return std::make_shared<IdentifierNode>(consumeIdentifier());
    }
    else {
        return parseLiteral();
    }
}

std::shared_ptr<AstNode> Parser::parseLiteral() {
    if (matchToken(TokenType::LITERAL)) {
        return std::make_shared<LiteralNode>(consumeToken());
    }
    else {
        throw std::runtime_error("Unexpected token");
    }
}

std::vector<std::shared_ptr<AstNode>> Parser::parseParameterList() {
    std::vector<std::shared_ptr<AstNode>> parameters;
    if (!matchToken(")")) {
        parameters.push_back(parseParameter());
        while (matchToken(",")) {
            consumeToken(",");
            parameters.push_back(parseParameter());
        }
    }
    return parameters;
}

std::shared_ptr<AstNode> Parser::parseParameter() {
    std::shared_ptr<ParameterNode> parameter = std::make_shared<ParameterNode>();
    parameter->type = consumeType();
    parameter->identifier = consumeIdentifier();
    return parameter;
}

std::string Parser::consumeIdentifier() {
    if (matchToken(TokenType::IDENTIFIER)) {
        return consumeToken();
    }
    else {
        throw std::runtime_error("Expected identifier");
    }
}

std::string Parser::consumeType() {
    if (matchKeyword("цело") || matchKeyword("вещественно") || matchKeyword("строка") || matchKeyword("истина")) {
        return consumeToken();
    }
    else {
        throw std::runtime_error("Expected type");
    }
}

std::string Parser::consumeToken() {
    return consumeToken(tokens[position].value);
}

std::string Parser::consumeToken(const std::string& expectedValue) {
    if (tokens[position].value == expectedValue) {
        return tokens[position++].value;
    }
    else {
        throw std::runtime_error("Unexpected token: " + tokens[position].value);
    }
}

bool Parser::matchToken(const std::string& expectedValue) {
    return tokens[position].value == expectedValue;
}

bool Parser::matchToken(TokenType expectedType) {
    return tokens[position].type == expectedType;
}

bool Parser::matchKeyword(const std::string& keyword) {
    return tokens[position].type == TokenType::KEYWORD && tokens[position].value == keyword;
}
