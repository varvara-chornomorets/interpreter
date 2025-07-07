#include <iostream>
#include <string>

class SimpleCalculator {
private:
    bool hasError = false; // error flag

public:
    double evaluate(const std::string& expression) {
        hasError = false; // reset error flag

        // remove spaces
        std::string expr = removeSpaces(expression);

        // validate input first
        if (!isValidInput(expr)) {
            hasError = true;
            return 0;
        }

        // parse and calculate
        return parseExpression(expr);
    }

    bool getError() const {
        return hasError;
    }

    std::string removeSpaces(const std::string& str) {
        std::string result;
        for (char c : str) {
            if (c != ' ') {
                result += c;
            }
        }
        return result;
    }

    bool isValidInput(const std::string& expr) {
        if (expr.empty()) return false;

        // check if expression contains only digits, +, -, *, and /
        for (char c : expr) {
            if (!isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/') {
                return false;
            }
        }

        // check if it starts with digit or minus, and ends with a digit
        if ((!isdigit(expr[0]) && expr[0] != '-') || !isdigit(expr.back())) {
            return false;
        }

        // check for consecutive operators (but allow minus after * or /)
        for (size_t i = 0; i < expr.length() - 1; i++) {
            char current = expr[i];
            char next = expr[i + 1];

            // no operator after operator, except minus after * or /
            if ((current == '+' || current == '-' || current == '*' || current == '/') &&
                (next == '+' || next == '*' || next == '/')) {
                return false;
            }

            // minus after plus is not allowed
            if (current == '+' && next == '-') {
                return false;
            }

            // minus after minus is not allowed  
            if (current == '-' && next == '-') {
                return false;
            }

            // minus after * or / is allowed (for negative numbers)
            if ((current == '*' || current == '/') && next == '-') {
                continue; // this is ok
            }
        }

        return true;
    }

    double parseExpression(const std::string& expr) {
        // parse addition and subtractiion
        size_t pos = 0;
        return parseAddSub(expr, pos);
    }

    double parseAddSub(const std::string& expr, size_t& pos) {
        double result = parseMulDiv(expr, pos);

        while (pos < expr.length() && (expr[pos] == '+' || expr[pos] == '-')) {
            char op = expr[pos];
            pos++; // skip operator
            double right = parseMulDiv(expr, pos);

            if (op == '+') {
                result += right;
            }
            else {
                result -= right;
            }
        }

        return result;
    }

    double parseMulDiv(const std::string& expr, size_t& pos) {
        double result = parseNumber(expr, pos);

        while (pos < expr.length() && (expr[pos] == '*' || expr[pos] == '/')) {
            char op = expr[pos];
            pos++; // skip operator
            double right = parseNumber(expr, pos);

            if (op == '*') {
                result *= right;
            }
            else {
                if (right == 0) {
                    std::cout << "Error: Division by zero!" << std::endl;
                    hasError = true;
                    return 0;
                }
                result /= right;
            }
        }

        return result;
    }

    double parseNumber(const std::string& expr, size_t& pos) {
        std::string numberStr = "";

        // handle negative sign at beginning or after * or /
        if (pos < expr.length() && expr[pos] == '-') {
            if (pos == 0 || expr[pos - 1] == '*' || expr[pos - 1] == '/') {
                numberStr += '-';
                pos++;
            }
        }

        // parse digits only
        while (pos < expr.length() && isdigit(expr[pos])) {
            numberStr += expr[pos];
            pos++;
        }

        if (numberStr.empty() || numberStr == "-") {
            return 0;
        }

        return std::stod(numberStr);
    }
};

int main() {
    SimpleCalculator calculator;
    std::string input;

    std::cout << "Enter expressions or 'q' to exit:" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "q") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        double result = calculator.evaluate(input);

        if (calculator.getError()) {
            std::cout << "Invalid input! Please use only whole numbers, +, -, *, and /" << std::endl;
            std::cout << "Examples: 5+3, -10+25, 6*7, 15/3, 5*-3, 10/-2" << std::endl;
        }
        else {
            std::cout << result << std::endl;
        }
    }

    return 0;
}

