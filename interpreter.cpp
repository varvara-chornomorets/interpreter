#include <iostream>
#include <string>

class SimpleCalculator {
public:
    long evaluate(const std::string& expression) {
        // remove spaces
        std::string expr = removeSpaces(expression);

        // validate input first
        if (!isValidInput(expr)) {
            return -1; // error flag
        }

        // parse and calculate
        return parseExpression(expr);
    }

private:
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

        // check if it starts and ends with a digit
        if (!isdigit(expr[0]) || !isdigit(expr.back())) {
            return false;
        }

        // check for consecutive operators
        for (size_t i = 0; i < expr.length() - 1; i++) {
            if ((expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') &&
                (expr[i + 1] == '+' || expr[i + 1] == '-' || expr[i + 1] == '*' || expr[i + 1] == '/')) {
                return false;
            }
        }

        return true;
    }

    long parseExpression(const std::string& expr) {
        long result = 0;
        long currentNumber = 0;
        char operation = '+'; // start with addition

        size_t i = 0;

        while (i < expr.length()) {
            // parse a number
            currentNumber = parseNumber(expr, i);

            // apply the operation
            if (operation == '+') {
                result += currentNumber;
            }
            else if (operation == '-') {
                result -= currentNumber;
            }
            else if (operation == '*') {
                result *= currentNumber;
            }
            else if (operation == '/') {
                if (currentNumber == 0) {
                    std::cout << "Error: Division by zero!" << std::endl;
                    return -1;
                }
                result /= currentNumber;
            }

            // get next operator if exists
            if (i < expr.length() && (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')) {
                operation = expr[i];
                i++;
            }
        }

        return result;
    }

    long parseNumber(const std::string& expr, size_t& pos) {
        std::string numberStr = "";

        // parse digits only
        while (pos < expr.length() && isdigit(expr[pos])) {
            numberStr += expr[pos];
            pos++;
        }

        if (numberStr.empty()) {
            return 0;
        }

        return std::stol(numberStr);
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

        long result = calculator.evaluate(input);

        if (result == -1) {
            std::cout << "Invalid input! Please use only whole numbers, +, -, *, and /" << std::endl;
            std::cout << "Examples: 5+3, 100-25, 6*7, 15/3" << std::endl;
        }
        else {
            std::cout << result << std::endl;
        }
    }

    return 0;
}

