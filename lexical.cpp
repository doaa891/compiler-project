#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>

// List of token types
enum TokenType {
    UNKNOWN,
    END,
    IDENTIFIER,
    INTEGER,
    OPERATOR,
    KEYWORD,
    DELIMITER
};


// Structure for token
struct Token {
    TokenType type;
    std::string value;


    Token(TokenType type, std::string value) : type(type), value(value) {}
};


// Function to check if a character is a delimiter
bool isDelimiter(char c) {
    std::string delimiters = "[]{}(),.;:";
    return delimiters.find(c) != std::string::npos;
}


// Function to check if a character is an operator
bool isOperator(char c) {
    std::string operators = "+-*/%";
    return operators.find(c) != std::string::npos;
}


// Function to check if a character is a keyword
bool isKeyword(const std::string &word) {
    std::vector<std::string> keywords = {"if", "else", "while", "return"};
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}


// Function to generate tokens from the input string
std::vector<Token> tokenize(const std::string &input) {
    std::vector<Token> tokens;
    std::string buffer;
    TokenType type = UNKNOWN;


    for (char c : input) {
        if (isspace(c)) {
            if (!buffer.empty()) {
                tokens.push_back(Token(type, buffer));
                buffer.clear();
            }
            continue;
        }


        if (isdigit(c)) {
            type = INTEGER;
        } else if (isalpha(c)) {
            type = IDENTIFIER;
        } else if (isDelimiter(c)) {
            type = DELIMITER;
        } else if (isOperator(c)) {
            type = OPERATOR;
        }


        buffer += c;


        if (type == KEYWORD && isKeyword(buffer)) {
            tokens.push_back(Token(KEYWORD, buffer));
            buffer.clear();
        }
    }


    if (!buffer.empty()) {
        tokens.push_back(Token(type, buffer));
    }


    return tokens;
}


int main() {
    std::string input;
    std::cout << "Enter the input string: ";
    std::getline(std::cin, input);


    std::vector<Token> tokens = tokenize(input);


    std::cout << "Tokens:" << std::endl;
    for (const Token &token : tokens) {
        std::cout << "Type: " << token.type << ", Value: " << token.value << std::endl;
    }


    return 0;
}