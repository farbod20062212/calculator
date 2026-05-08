#include <iostream>
#define M_PI 3.14159265358979323846
#include <cmath>
#include <iomanip>
#include <limits>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

class Calculator {
private:
    double memory;
    double lastResult;
    bool lastResultValid;      // true if lastResult holds a valid previous result
    bool radiansMode;          // false = degrees, true = radians
    int precision;             // decimal places for output
    vector<string> history;    // stores operation descriptions

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    double getNumber(const string& prompt) {
        double num;
        while (true) {
            cout << prompt;
            cin >> num;
            if (cin.fail()) {
                cout << "Invalid input! Please enter a valid number.\n";
                clearInput();
            }
            else {
                clearInput();
                return num;
            }
        }
    }

    char getOperator() {
        char op;
        while (true) {
            cout << "\nEnter operator (+, -, *, /, ^, %,\n"
                << "  v=sqrt, s=sin, c=cos, t=tan, !=factorial,\n"
                << "  l=log10, n=ln, e=exp, d=mode, p=prec, h=hist, u=undo,\n"
                << "  m=memory, r=reset): ";
            cin >> op;
            clearInput();
            op = tolower(op);
            // valid commands list
            if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^' ||
                op == '%' || op == 'v' || op == 's' || op == 'c' || op == 't' ||
                op == '!' || op == 'l' || op == 'n' || op == 'e' || op == 'd' ||
                op == 'p' || op == 'h' || op == 'u' || op == 'm' || op == 'r') {
                return op;
            }
            cout << "Invalid operator! Please try again.\n";
        }
    }

    // Basic binary operations
    double add(double a, double b) { return a + b; }
    double subtract(double a, double b) { return a - b; }
    double multiply(double a, double b) { return a * b; }
    double divide(double a, double b) {
        if (b == 0) throw runtime_error("Division by zero error!");
        return a / b;
    }
    double power(double a, double b) { return pow(a, b); }
    double modulo(double a, double b) {
        if (b == 0) throw runtime_error("Modulo by zero error!");
        return fmod(a, b);
    }

    // Unary scientific functions (degrees/radians handled internally)
    double squareRoot(double a) {
        if (a < 0) throw runtime_error("Cannot calculate square root of negative number!");
        return sqrt(a);
    }

    double sine(double a) {
        double rad = radiansMode ? a : a * M_PI / 180.0;
        return sin(rad);
    }
    double cosine(double a) {
        double rad = radiansMode ? a : a * M_PI / 180.0;
        return cos(rad);
    }
    double tangent(double a) {
        double rad = radiansMode ? a : a * M_PI / 180.0;
        if (fabs(cos(rad)) < 1e-9)   // fixed floating‑point comparison
            throw runtime_error("Tangent undefined at this angle!");
        return tan(rad);
    }

    double factorial(double a) {
        if (a < 0) throw runtime_error("Factorial of negative number is undefined!");
        if (a != floor(a)) throw runtime_error("Factorial only defined for integers!");
        int n = static_cast<int>(a);
        double result = 1;
        for (int i = 2; i <= n; ++i) result *= i;
        return result;
    }

    double logarithm10(double a) {
        if (a <= 0) throw runtime_error("Logarithm of non-positive number!");
        return log10(a);
    }
    double naturalLog(double a) {
        if (a <= 0) throw runtime_error("Natural logarithm of non-positive number!");
        return log(a);
    }
    double exponential(double a) {
        return exp(a);
    }

    // Save previous result for undo
    void saveCurrentResult(double current, bool hasResult) {
        if (hasResult) {
            lastResult = current;
            lastResultValid = true;
        }
        else {
            lastResultValid = false;
        }
    }

    // Add an entry to the history vector
    void addToHistory(const string& entry) {
        history.push_back(entry);
        // Optional: limit history size
        if (history.size() > 20) history.erase(history.begin());
    }

public:
    Calculator() : memory(0), lastResult(0), lastResultValid(false),
        radiansMode(false), precision(6) {}

    void run() {
        cout << "\n========================================\n";
        cout << "      ADVANCED CALCULATOR\n";
        cout << "========================================\n";
        cout << "Basic: +, -, *, /, ^, %\n";
        cout << "Scientific: sqrt(v), sin(s), cos(c), tan(t), factorial(!)\n";
        cout << "Log/Exp: log10(l), ln(n), exp(e)\n";
        cout << "Mode: d (toggles between DEG and RAD)\n";
        cout << "Precision: p (set decimal places)\n";
        cout << "History: h (show last operations)\n";
        cout << "Undo: u (restore previous result)\n";
        cout << "Memory: m (store/recall/clear), Reset: r\n";
        cout << "========================================\n\n";

        double result = 0;
        bool hasResult = false;

        while (true) {
            cout << "\n----------------------------------------\n";
            if (hasResult) {
                cout << "Current result: " << fixed << setprecision(precision) << result << endl;
            }
            if (memory != 0 || true) { // always show memory line for clarity
                cout << "Memory: " << fixed << setprecision(precision) << memory << "   ";
            }
            cout << "Mode: " << (radiansMode ? "RAD" : "DEG") << "   Precision: " << precision << endl;

            char op = getOperator();

            // --- Memory operations ---
            if (op == 'm') {
                char memOp;
                cout << "Memory operations: (s)tore, (r)ecall, (c)lear: ";
                cin >> memOp;
                clearInput();
                memOp = tolower(memOp);

                if (memOp == 's') {
                    if (hasResult) {
                        memory = result;
                        cout << "Stored " << result << " to memory.\n";
                    }
                    else {
                        double val = getNumber("Enter value to store: ");
                        memory = val;
                        cout << "Stored " << val << " to memory.\n";
                    }
                }
                else if (memOp == 'r') {
                    if (memory != 0 || hasResult) {
                        saveCurrentResult(result, hasResult);   // for undo
                        result = memory;
                        hasResult = true;
                        cout << "Recalled " << memory << " from memory.\n";
                    }
                    else {
                        cout << "Memory is empty!\n";
                    }
                }
                else if (memOp == 'c') {
                    memory = 0;
                    cout << "Memory cleared.\n";
                }
                else {
                    cout << "Invalid memory operation!\n";
                }
                // fall through to continue prompt
            }
            // --- Reset ---
            else if (op == 'r') {
                result = 0;
                hasResult = false;
                memory = 0;
                lastResultValid = false;
                history.clear();
                cout << "Calculator cleared!\n";
            }
            // --- Toggle angle mode ---
            else if (op == 'd') {
                radiansMode = !radiansMode;
                cout << "Angle mode set to " << (radiansMode ? "RADIANS" : "DEGREES") << endl;
            }
            // --- Set precision ---
            else if (op == 'p') {
                int p = getNumber("Enter decimal places (0-15): ");
                if (p >= 0 && p <= 15) {
                    precision = p;
                    cout << "Precision set to " << precision << endl;
                }
                else {
                    cout << "Invalid precision. Must be between 0 and 15.\n";
                }
            }
            // --- Show history ---
            else if (op == 'h') {
                if (history.empty()) {
                    cout << "No operations recorded yet.\n";
                }
                else {
                    cout << "\n--- Operation History ---\n";
                    for (size_t i = 0; i < history.size(); ++i)
                        cout << i + 1 << ": " << history[i] << endl;
                    cout << "-------------------------\n";
                }
            }
            // --- Undo ---
            else if (op == 'u') {
                if (lastResultValid) {
                    result = lastResult;
                    lastResultValid = false;   // undo only once
                    hasResult = true;
                    cout << "Undo successful. Restored " << result << endl;
                }
                else {
                    cout << "Nothing to undo.\n";
                }
            }
            // --- Unary operations ---
            else if (op == 'v' || op == 's' || op == 'c' || op == 't' ||
                op == '!' || op == 'l' || op == 'n' || op == 'e') {
                double num;
                if (hasResult) {
                    cout << "Use current result (" << result << ")? (y/n): ";
                    char choice;
                    cin >> choice;
                    clearInput();
                    if (tolower(choice) == 'y') {
                        num = result;
                    }
                    else {
                        num = getNumber("Enter number: ");
                    }
                }
                else {
                    num = getNumber("Enter number: ");
                }

                try {
                    saveCurrentResult(result, hasResult);   // save before change
                    string histStr;
                    switch (op) {
                    case 'v':
                        result = squareRoot(num);
                        histStr = "sqrt(" + to_string(num) + ") = " + to_string(result);
                        break;
                    case 's':
                        result = sine(num);
                        histStr = "sin(" + to_string(num) + ") = " + to_string(result);
                        break;
                    case 'c':
                        result = cosine(num);
                        histStr = "cos(" + to_string(num) + ") = " + to_string(result);
                        break;
                    case 't':
                        result = tangent(num);
                        histStr = "tan(" + to_string(num) + ") = " + to_string(result);
                        break;
                    case '!':
                        result = factorial(num);
                        histStr = "fact(" + to_string(num) + ") = " + to_string(result);
                        break;
                    case 'l':
                        result = logarithm10(num);
                        histStr = "log10(" + to_string(num) + ") = " + to_string(result);
                        break;
                    case 'n':
                        result = naturalLog(num);
                        histStr = "ln(" + to_string(num) + ") = " + to_string(result);
                        break;
                    case 'e':
                        result = exponential(num);
                        histStr = "exp(" + to_string(num) + ") = " + to_string(result);
                        break;
                    }
                    hasResult = true;
                    addToHistory(histStr);
                    cout << "Result: " << fixed << setprecision(precision) << result << endl;
                }
                catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
            // --- Binary operations ---
            else {
                double num1, num2;
                if (hasResult) {
                    cout << "Use current result as first number? (y/n): ";
                    char choice;
                    cin >> choice;
                    clearInput();
                    if (tolower(choice) == 'y') {
                        num1 = result;
                        num2 = getNumber("Enter second number: ");
                    }
                    else {
                        num1 = getNumber("Enter first number: ");
                        num2 = getNumber("Enter second number: ");
                    }
                }
                else {
                    num1 = getNumber("Enter first number: ");
                    num2 = getNumber("Enter second number: ");
                }

                try {
                    saveCurrentResult(result, hasResult);   // save for undo
                    string histStr;
                    switch (op) {
                    case '+': result = add(num1, num2); break;
                    case '-': result = subtract(num1, num2); break;
                    case '*': result = multiply(num1, num2); break;
                    case '/': result = divide(num1, num2); break;
                    case '^': result = power(num1, num2); break;
                    case '%': result = modulo(num1, num2); break;
                    }
                    histStr = to_string(num1) + " " + op + " " + to_string(num2) + " = " + to_string(result);
                    hasResult = true;
                    addToHistory(histStr);
                    cout << "\n" << num1 << " " << op << " " << num2 << " = "
                        << fixed << setprecision(precision) << result << endl;
                }
                catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }

            // Ask to continue after EVERY operation (fixes the missing exit bug)
            cout << "\nContinue calculating? (y/n): ";
            char cont;
            cin >> cont;
            clearInput();
            if (tolower(cont) != 'y') {
                cout << "\nThank you for using the calculator!\n";
                break;
            }
        }
    }
};

int main() {
    Calculator calc;
    calc.run();
    return 0;
}