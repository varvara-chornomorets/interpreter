#include <iostream>
#include <string>

class SimpleCalculator {
public:
    long evaluate(const std::string& expression) {
        // remove spaces
        std::string expr = removeSpaces(expression);

        // check input
        if (!isValidInput(expr)) {
            return -1; // error to be cathced
        }

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

        for (char c : expr) {
            if (!isdigit(c) && c != '+' && c != '-') {
                return false;
            }
        }
        if (!isdigit(expr[0]) || !isdigit(expr.back())) {
            return false;
        }
        for (size_t i = 0; i < expr.length() - 1; i++) {
            if ((expr[i] == '+' || expr[i] == '-') &&
                (expr[i + 1] == '+' || expr[i + 1] == '-')) {
                return false;
            }
        }

        return true;
    }

    long parseExpression(const std::string& expr) {
        long result = 0;
        long currentNumber = 0;
        char operation = '+'; 

        size_t i = 0;

        while (i < expr.length()) {
            currentNumber = parseNumber(expr, i);

            if (operation == '+') {
                result += currentNumber;
            }
            else if (operation == '-') {
                result -= currentNumber;
            }
            // get next operator

            if (i < expr.length() && (expr[i] == '+' || expr[i] == '-')) {
                operation = expr[i];
                i++;
            }
        }

        return result;
    }

    long parseNumber(const std::string& expr, size_t& pos) {
        std::string numberStr = "";

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
            std::cout << "Invalid input! Please use only whole numbers, +, and -" << std::endl;
            std::cout << "Examples: 5+3, 100-25, 123+456-78" << std::endl;
        }
        else {
            std::cout << result << std::endl;
        }
    }

    return 0;
}

/*
Valid examples:
> 5+3
8

> 100-25
75

> 123456+789012
912468

> 1000000-500000
500000

Invalid examples (will ask for new input):
> 5.5+3    (decimal numbers)
> 5*3      (multiplication)
> +5       (starts with operator)
> 5++3     (consecutive operators)
> abc      (letters)
*/