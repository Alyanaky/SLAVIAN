#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include "lexer.h"

enum class NodeType {
    PROGRAM,
    DECLARATION,
    ASSIGNMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FUNCTION_DECLARATION,
    RETURN_STATEMENT,
    EXPRESSION,
    TERM,
    FACTOR,
    LITERAL,
    IDENTIFIER,
    PARAMETER_LIST,
    PARAMETER
};

class AstNode {
public:
    virtual ~AstNode() = default;
    virtual NodeType getType() const = 0;
};

class ProgramNode : public AstNode {
public:
    std::vector<std::shared_ptr<AstNode>> statements;
    NodeType getType() const override { return NodeType::PROGRAM; }
};

class DeclarationNode : public AstNode {
public:
    std::string identifier;
    std::string type;
    std::shared_ptr<AstNode> expression;
    NodeType getType() const override { return NodeType::DECLARATION; }
};

class AssignmentNode : public AstNode {
public:
    std::string identifier;
    std::shared_ptr<AstNode> expression;
    NodeType getType() const override { return NodeType::ASSIGNMENT; }
};

class IfStatementNode : public AstNode {
public:
    std::shared_ptr<AstNode> condition;
    std::vector<std::shared_ptr<AstNode>> thenStatements;
    std::vector<std::shared_ptr<AstNode>> elseStatements;
    NodeType getType() const override { return NodeType::IF_STATEMENT; }
};

class WhileStatementNode : public AstNode {
public:
    std::shared_ptr<AstNode> condition;
    std::vector<std::shared_ptr<AstNode>> statements;
    NodeType getType() const override { return NodeType::WHILE_STATEMENT; }
};

class FunctionDeclarationNode : public AstNode {
public:
    std::string identifier;
    std::vector<std::shared_ptr<AstNode>> parameters;
    std::vector<std::shared_ptr<AstNode>> statements;
    NodeType getType() const override { return NodeType::FUNCTION_DECLARATION; }
};

class ReturnStatementNode : public AstNode {
public:
    std::shared_ptr<AstNode> expression;
    NodeType getType() const override { return NodeType::RETURN_STATEMENT; }
};

class ExpressionNode : public AstNode {
public:
    std::shared_ptr<AstNode> left;
    std::shared_ptr<AstNode> right;
    std::string op;
    NodeType getType() const override { return NodeType::EXPRESSION; }
};

class TermNode : public AstNode {
public:
    std::shared_ptr<AstNode> left;
    std::shared_ptr<AstNode> right;
    std::string op;
    NodeType getType() const override { return NodeType::TERM; }
};

class FactorNode : public AstNode {
public:
    std::shared_ptr<AstNode> expression;
    NodeType getType() const override { return NodeType::FACTOR; }
};

class LiteralNode : public AstNode {
public:
    std::string value;
    NodeType getType() const override { return NodeType::LITERAL; }
};

class IdentifierNode : public AstNode {
public:
    std::string name;
    NodeType getType() const override { return NodeType::IDENTIFIER; }
};

class ParameterListNode : public AstNode {
public:
    std::vector<std::shared_ptr<AstNode>> parameters;
    NodeType getType() const override { return NodeType::PARAMETER_LIST; }
};

class ParameterNode : public AstNode {
public:
    std::string type;
    std::string identifier;
    NodeType getType() const override { return NodeType::PARAMETER; }
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<AstNode> parse();

private:
    std::vector<Token> tokens;
    size_t position;

    std::shared_ptr<AstNode> parseProgram();
    std::shared_ptr<AstNode> parseStatement();
    std::shared_ptr<AstNode> parseDeclaration();
    std::shared_ptr<AstNode> parseAssignment();
    std::shared_ptr<AstNode> parseIfStatement();
    std::shared_ptr<AstNode> parseWhileStatement();
    std::shared_ptr<AstNode> parseFunctionDeclaration();
    std::shared_ptr<AstNode> parseReturnStatement();
    std::vector<std::shared_ptr<AstNode>> parseStatementList();
    std::shared_ptr<AstNode> parseExpression();
    std::shared_ptr<AstNode> parseTerm();
    std::shared_ptr<AstNode> parseFactor();
    std::shared_ptr<AstNode> parseLiteral();
    std::vector<std::shared_ptr<AstNode>> parseParameterList();
    std::shared_ptr<AstNode> parseParameter();
    std::string consumeIdentifier();
    std::string consumeType();
    std::string consumeToken();
    std::string consumeToken(const std::string& expectedValue);
    bool matchToken(const std::string& expectedValue);
    bool matchToken(TokenType expectedType);
    bool matchKeyword(const std::string& keyword);
};

#endif // PARSER_H
