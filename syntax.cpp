#include <iostream>
#include <memory>
#include <string>
#include <cctype>

enum class TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    OPEN_PAREN,
    CLOSE_PAREN,
    END_OF_FILE,
    INVALID
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), currentPos(0) {}

    Token getNextToken() {
        skipWhitespace();

        if (currentPos >= input.size()) {
            return {TokenType::END_OF_FILE, ""};
        }

        char currentChar = input[currentPos];
        switch (currentChar) {
            case '+': return createToken(TokenType::PLUS);
            case '-': return createToken(TokenType::MINUS);
            case '*': return createToken(TokenType::MULTIPLY);
            case '/': return createToken(TokenType::DIVIDE);
            case '(': return createToken(TokenType::OPEN_PAREN);
            case ')': return createToken(TokenType::CLOSE_PAREN);
            default:
                if (isdigit(currentChar)) {
                    return getNumber();
                }
                return {TokenType::INVALID, ""};
        }
    }

private:
    std::string input;
    size_t currentPos;

    void skipWhitespace() {
        while (currentPos < input.size() && isspace(input[currentPos])) {
            currentPos++;
        }
    }

    Token createToken(TokenType type) {
        std::string value(1, input[currentPos++]);
        return {type, value};
    }

    Token getNumber() {
        std::string result;
        while (currentPos < input.size() && isdigit(input[currentPos])) {
            result += input[currentPos++];
        }
        return {TokenType::INTEGER, result};
    }
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

class BinOpNode : public ASTNode {
public:
    BinOpNode(TokenType op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "BinaryOp: ";
        switch (op) {
            case TokenType::PLUS: std::cout << "+" << std::endl; break;
            case TokenType::MINUS: std::cout << "-" << std::endl; break;
            case TokenType::MULTIPLY: std::cout << "*" << std::endl; break;
            case TokenType::DIVIDE: std::cout << "/" << std::endl; break;
            default: std::cout << "Unknown" << std::endl;
        }
        left->print(indent + 1);
        right->print(indent + 1);
    }

private:
    TokenType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    void printIndent(int indent) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};

class NumNode : public ASTNode {
public:
    NumNode(int value) : value(value) {}

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "Number: " << value << std::endl;
    }

private:
    int value;

    void printIndent(int indent) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }
};

class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.getNextToken()) {}

    std::unique_ptr<ASTNode> parse() {
        return expr();
    }

private:
    Lexer& lexer;
    Token currentToken;

    void eat(TokenType expectedType) {
        if (currentToken.type == expectedType) {
            currentToken = lexer.getNextToken();
        }
    }

    std::unique_ptr<ASTNode> factor() {
        Token token = currentToken;

        if (token.type == TokenType::OPEN_PAREN) {
            eat(TokenType::OPEN_PAREN);
            auto result = expr();
            eat(TokenType::CLOSE_PAREN);
            return result;
        }

        if (token.type == TokenType::INTEGER) {
            eat(TokenType::INTEGER);
            return std::make_unique<NumNode>(std::stoi(token.value));
        }

        return nullptr;
    }

    std::unique_ptr<ASTNode> term() {
        auto result = factor();
        Token token = currentToken;

        while (token.type == TokenType::MULTIPLY || token.type == TokenType::DIVIDE) {
            eat(token.type);
            auto right = factor();
            result = std::make_unique<BinOpNode>(token.type, std::move(result), std::move(right));
            token = currentToken;
        }

        return result;
    }

    std::unique_ptr<ASTNode> expr() {
        auto result = term();
        Token token = currentToken;

        while (token.type == TokenType::PLUS || token.type == TokenType::MINUS) {
            eat(token.type);
            auto right = term();
            result = std::make_unique<BinOpNode>(token.type, std::move(result), std::move(right));
            token = currentToken;
        }

        return result;
    }
};

int main() {
    std::string input;
    std::cout << "Enter an arithmetic expression: ";
    std::getline(std::cin, input);

    Lexer lexer(input);
    Parser parser(lexer);
    auto syntaxTree = parser.parse();

    std::cout << "Syntax Tree:" << std::endl;
    syntaxTree->print();

    return 0;
}