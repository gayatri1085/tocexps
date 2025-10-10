#include <iostream>
#include <stack>
#include <string>
using namespace std;

// Function to check if a character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Function to return precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Convert infix to postfix using stack
string infixToPostfix(string infix) {
    stack<char> s;
    string postfix = "";

    for (char c : infix) {
        if (c == ' ') continue;

        // If operand, add to output
        if (isalnum(c)) {
            postfix += c;
            postfix += ' ';
        }
        // If '(', push to stack
        else if (c == '(') {
            s.push(c);
        }
        // If ')', pop until '('
        else if (c == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                postfix += ' ';
                s.pop();
            }
            if (!s.empty()) s.pop();
        }
        // Operator
        else if (isOperator(c)) {
            while (!s.empty() && precedence(s.top()) >= precedence(c)) {
                postfix += s.top();
                postfix += ' ';
                s.pop();
            }
            s.push(c);
        }
    }

    // Pop remaining operators
    while (!s.empty()) {
        postfix += s.top();
        postfix += ' ';
        s.pop();
    }

    return postfix;
}

// Generate stack machine assembly from postfix expression
void generateAssembly(string postfix) {
    stack<string> st;
    cout << "\nGenerated Assembly Code:\n";

    for (size_t i = 0; i < postfix.size(); i++) {
        char c = postfix[i];
        if (c == ' ') continue;

        if (isalnum(c)) {
            cout << "PUSH " << c << endl;
            st.push(string(1, c));
        }
        else if (isOperator(c)) {
            string op2 = st.top(); st.pop();
            string op1 = st.top(); st.pop();

            switch (c) {
                case '+': cout << "ADD" << endl; break;
                case '-': cout << "SUB" << endl; break;
                case '*': cout << "MUL" << endl; break;
                case '/': cout << "DIV" << endl; break;
            }

            // Result is pushed back (not used but conceptually correct)
            st.push("T");
        }
    }
}

int main() {
    string expr;
    cout << "Enter arithmetic expression: ";
    getline(cin, expr);

    cout << "\nInput Expression: " << expr << endl;

    string postfix = infixToPostfix(expr);
    cout << "\nPostfix Expression: " << postfix << endl;

    generateAssembly(postfix);

    return 0;
}
