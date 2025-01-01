#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "lexer.h"
#include "parser.h"

void printAst(const std::shared_ptr<AstNode>& node, int depth = 0) {
    std::string indent(depth, ' ');
    std::cout << indent << "Node: " << static_cast<int>(node->getType()) << std::endl;
    if (auto program = std::dynamic_pointer_cast<ProgramNode>(node)) {
        for (const auto& statement : program->statements) {
            printAst(statement, depth + 2);
        }
    }
    else if (auto declaration = std::dynamic_pointer_cast<DeclarationNode>(node)) {
        std::cout << indent << "  Identifier: " << declaration->identifier << std::endl;
        std::cout << indent << "  Type: " << declaration->type << std::endl;
        printAst(declaration->expression, depth + 2);
    }
    else if (auto assignment = std::dynamic_pointer_cast<AssignmentNode>(node)) {
        std::cout << indent << "  Identifier: " << assignment->identifier << std::endl;
        printAst(assignment->expression, depth + 2);
    }
    else if (auto ifStatement = std::dynamic_pointer_cast<IfStatementNode>(node)) {
        printAst(ifStatement->condition, depth + 2);
        for (const auto& statement : ifStatement->thenStatements) {
            printAst(statement, depth + 2);
        }
        for (const auto& statement : ifStatement->elseStatements) {
            printAst(statement, depth + 2);
        }
    }
    else if (auto whileStatement = std::dynamic_pointer_cast<WhileStatementNode>(node)) {
        printAst(whileStatement->condition, depth + 2);
        for (const auto& statement : whileStatement->statements) {
            printAst(statement, depth + 2);
        }
    }
    else if (auto function = std::dynamic_pointer_cast<FunctionDeclarationNode>(node)) {
        std::cout << indent << "  Identifier: " << function->identifier << std::endl;
        for (const auto& parameter : function->parameters) {
            printAst(parameter, depth + 2);
        }
        for (const auto& statement : function->statements) {
            printAst(statement, depth + 2);
        }
    }
    else if (auto returnStatement = std::dynamic_pointer_cast<ReturnStatementNode>(node)) {
        printAst(returnStatement->expression, depth + 2);
    }
    else if (auto expression = std::dynamic_pointer_cast<ExpressionNode>(node)) {
        printAst(expression->left, depth + 2);
        printAst(expression->right, depth + 2);
        std::cout << indent << "  Operator: " << expression->op << std::endl;
    }
    else if (auto term = std::dynamic_pointer_cast<TermNode>(node)) {
        printAst(term->left, depth + 2);
        printAst(term->right, depth + 2);
        std::cout << indent << "  Operator: " << term->op << std::endl;
    }
    else if (auto factor = std::dynamic_pointer_cast<FactorNode>(node)) {
        printAst(factor->expression, depth + 2);
    }
    else if (auto literal = std::dynamic_pointer_cast<LiteralNode>(node)) {
        std::cout << indent << "  Value: " << literal->value << std::endl;
    }
    else if (auto identifier = std::dynamic_pointer_cast<IdentifierNode>(node)) {
        std::cout << indent << "  Name: " << identifier->name << std::endl;
    }
    else if (auto parameterList = std::dynamic_pointer_cast<ParameterListNode>(node)) {
        for (const auto& parameter : parameterList->parameters) {
            printAst(parameter, depth + 2);
        }
    }
    else if (auto parameter = std::dynamic_pointer_cast<ParameterNode>(node)) {
        std::cout << indent << "  Type: " << parameter->type << std::endl;
        std::cout << indent << "  Identifier: " << parameter->identifier << std::endl;
    }
}

int main() {
    std::string source = R"(
        даждь число цело = 10;
        аще (число > 5) {
            воздать "число больше 5";
        } иначе {
            воздать "число меньше или равно 5";
        }
    )";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::shared_ptr<AstNode> ast = parser.parse();

    // Вывод AST (для простоты выводим только типы узлов)
    printAst(ast);

    return 0;
}
