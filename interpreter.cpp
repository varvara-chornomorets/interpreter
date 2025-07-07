#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// token structure
struct Token {
    enum Type { NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, END };
    Type type;
    double value;

    Token(Type t, double v = 0.0) : type(t), value(v) {}
};

// pure function: remove spaces
std::string removeSpaces(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
        [](char c) { return c != ' '; });
    return result;
}

// pure function: validate input
bool isValidInput(const std::string& expr) {
    if (expr.empty()) return false;

    // check all characters are valid
    auto isValidChar = [](char c) {
        return std::isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/';
        };
    if (!std::all_of(expr.begin(), expr.end(), isValidChar)) {
        return false;
    }

    // check start and end
    if ((!std::isdigit(expr[0]) && expr[0] != '-') || !std::isdigit(expr.back())) {
        return false;
    }

    // check for invalid operator sequences
    for (size_t i = 0; i < expr.length() - 1; i++) {
        char current = expr[i];
        char next = expr[i + 1];

        auto isOperator = [](char c) { return c == '+' || c == '-' || c == '*' || c == '/'; };

        if (isOperator(current) && (next == '+' || next == '*' || next == '/')) {
            return false;
        }

        // minus after plus or minus not allowed
        if ((current == '+' || current == '-') && next == '-') {
            return false;
        }
    }

    return true;
}

std::vector<Token> tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    size_t pos = 0;

    while (pos < expr.length()) {
        if (std::isdigit(expr[pos]) ||
            (expr[pos] == '-' && (pos == 0 || expr[pos - 1] == '*' || expr[pos - 1] == '/'))) {

            // parse number
            std::string numberStr;
            if (expr[pos] == '-') {
                numberStr += '-';
                pos++;
            }

            while (pos < expr.length() && std::isdigit(expr[pos])) {
                numberStr += expr[pos];
                pos++;
            }

            if (!numberStr.empty() && numberStr != "-") {
                tokens.emplace_back(Token::NUMBER, std::stod(numberStr));
            }
        }
        else {
            // parse operator
            switch (expr[pos]) {
            case '+': tokens.emplace_back(Token::PLUS); break;
            case '-': tokens.emplace_back(Token::MINUS); break;
            case '*': tokens.emplace_back(Token::MULTIPLY); break;
            case '/': tokens.emplace_back(Token::DIVIDE); break;
            }
            pos++;
        }
    }

    tokens.emplace_back(Token::END);
    return tokens;
}

double applyOperation(double left, Token::Type op, double right) {
    switch (op) {
    case Token::PLUS: return left + right;
    case Token::MINUS: return left - right;
    case Token::MULTIPLY: return left * right;
    case Token::DIVIDE: return left / right;
    default: return 0.0;
    }
}

class Parser {
    const std::vector<Token>& tokens;
    mutable size_t position; 

public:
    Parser(const std::vector<Token>& toks) : tokens(toks), position(0) {}

    Token current() const {
        return position < tokens.size() ? tokens[position] : Token(Token::END);
    }

    // advance position
    void advance() const { position++; }

    // parse expression with proper priority
    std::pair<double, bool> parseExpression() const {
        return parseAddSub();
    }

private:
    // parse addition and subtraction 
    std::pair<double, bool> parseAddSub() const {
        auto leftResult = parseMulDiv();
        if (!leftResult.second) return std::make_pair(0.0, false);

        double result = leftResult.first;

        while (current().type == Token::PLUS || current().type == Token::MINUS) {
            Token::Type op = current().type;
            advance();

            auto rightResult = parseMulDiv();
            if (!rightResult.second) return std::make_pair(0.0, false);

            result = applyOperation(result, op, rightResult.first);
        }

        return std::make_pair(result, true);
    }

    // parse multiplication and division 
    std::pair<double, bool> parseMulDiv() const {
        auto leftResult = parseNumber();
        if (!leftResult.second) return std::make_pair(0.0, false);

        double result = leftResult.first;

        while (current().type == Token::MULTIPLY || current().type == Token::DIVIDE) {
            Token::Type op = current().type;
            advance();

            auto rightResult = parseNumber();
            if (!rightResult.second) return std::make_pair(0.0, false);

            if (op == Token::DIVIDE && rightResult.first == 0.0) {
                return std::make_pair(0.0, false); // division by zero
            }

            result = applyOperation(result, op, rightResult.first);
        }

        return std::make_pair(result, true);
    }

    // parse number
    std::pair<double, bool> parseNumber() const {
        if (current().type == Token::NUMBER) {
            double value = current().value;
            advance();
            return std::make_pair(value, true);
        }
        return std::make_pair(0.0, false);
    }
};

// pure function: evaluate expression
std::pair<double, bool> evaluate(const std::string& expression) {
    std::string cleanExpr = removeSpaces(expression);

    if (!isValidInput(cleanExpr)) {
        return std::make_pair(0.0, false);
    }

    std::vector<Token> tokens = tokenize(cleanExpr);
    Parser parser(tokens);

    auto result = parser.parseExpression();
    // check for division by zero

    if (!result.second) {
        return std::make_pair(0.0, false);
    }

    return result;
}

// pure function: process single calculation
void processCalculation(const std::string& input) {
    if (input.empty()) return;

    auto result = evaluate(input);

    if (!result.second) {
        std::cout << "Invalid input! Please use only whole numbers, +, -, *, and /" << std::endl;
        std::cout << "Examples: 5+3, -10+25, 6*7, 15/3, 5*-3, 10/-2" << std::endl;
    }
    else {
        std::cout << result.first << std::endl;
    }
}

int main() {
    std::string input;

    std::cout << "Functional Calculator (proper order of operations)" << std::endl;
    std::cout << "Enter expressions or 'q' to exit:" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "q") {
            break;
        }

        processCalculation(input);
    }

    return 0;
}
