#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Function to trim spaces
string trim(const string &str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to simplify an expression
string optimizeExpression(string expr) {
    // Remove spaces
    expr = trim(expr);

    // Strength reduction and constant folding
    if (expr.find('*') != string::npos) {
        stringstream ss(expr);
        string a, op, b;
        ss >> a >> op >> b;

        if (a == "1") return b;
        if (b == "1") return a;
        if (a == "0" || b == "0") return "0";

        // Constant folding for multiplication
        try {
            int x = stoi(a);
            int y = stoi(b);
            return to_string(x * y);
        } catch (...) {}
    }

    if (expr.find('+') != string::npos) {
        stringstream ss(expr);
        string a, op, b;
        ss >> a >> op >> b;

        if (a == "0") return b;
        if (b == "0") return a;

        // Constant folding for addition
        try {
            int x = stoi(a);
            int y = stoi(b);
            return to_string(x + y);
        } catch (...) {}
    }

    if (expr.find('-') != string::npos) {
        stringstream ss(expr);
        string a, op, b;
        ss >> a >> op >> b;
        if (b == "0") return a;
    }

    if (expr.find('/') != string::npos) {
        stringstream ss(expr);
        string a, op, b;
        ss >> a >> op >> b;
        if (b == "1") return a;
    }

    return expr;
}

int main() {
    vector<string> code = {
        "x = 2 * 8",
        "y = x * 1",
        "z = y + 0"
    };

    cout << "Input Code:\n";
    for (auto &line : code)
        cout << "    " << line << endl;

    cout << "\nOptimized Code:\n";
    vector<pair<string, string>> symbolTable;

    for (auto &line : code) {
        size_t eq = line.find('=');
        if (eq == string::npos) continue;

        string var = trim(line.substr(0, eq));
        string expr = trim(line.substr(eq + 1));

        // Optimize expression
        string optimizedExpr = optimizeExpression(expr);

        // Replace known values
        for (auto &entry : symbolTable) {
            size_t pos = optimizedExpr.find(entry.first);
            if (pos != string::npos) {
                optimizedExpr.replace(pos, entry.first.size(), entry.second);
            }
        }

        // Check if expression is a constant
        bool isNumber = true;
        for (char c : optimizedExpr)
            if (!isdigit(c)) { isNumber = false; break; }

        // Store optimized value
        symbolTable.push_back({var, optimizedExpr});

        // Skip dead code (if variable not used later)
        // For simplicity, we print only used ones in next expressions
    }

    // Final pass to remove dead code (simplified)
    // Only keep variables used later
    vector<string> optimizedOutput;
    for (int i = 0; i < symbolTable.size(); i++) {
        string var = symbolTable[i].first;
        string val = symbolTable[i].second;
        bool usedLater = false;

        for (int j = i + 1; j < symbolTable.size(); j++) {
            if (symbolTable[j].second.find(var) != string::npos) {
                usedLater = true;
                break;
            }
        }

        if (usedLater || i == symbolTable.size() - 1) {
            optimizedOutput.push_back(var + " = " + val);
        }
    }

    for (auto &line : optimizedOutput)
        cout << "    " << line << endl;

    return 0;
}
