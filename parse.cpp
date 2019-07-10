#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


struct formula {
    std::string value;
    formula * left;
    formula * right;
};

struct formula * parse(std::string formula) {

    std::cout << formula << std::endl;

    std::stringstream formulaStream(formula);

    std::string value;

    int nestLevel = 0;
    int andIndex = -1;
    int orIndex = -1;
    bool isLiteral = false;

    if (formula.front() == '(' && formula.back() == ')') {
        return parse(formula.substr(1, formula.length() - 2));
    }

    while(formulaStream >> value) {
        if (value.front() == '(') {
            if (nestLevel == 0 && andIndex != -1) {
                break;
            }
            nestLevel += (int)std::count(value.begin(), value.end(), '(');;
            continue;
        }
        if (value.back() == ')') {
            nestLevel -= (int)std::count(value.begin(), value.end(), ')');
            if (nestLevel == 0 && formulaStream.eof()) {
                return parse(formula.substr(1, formula.length() - 2));
            }
            continue;
        }
        if (value == ".") {
            if (nestLevel == 0 && andIndex == -1) {
                andIndex = formulaStream.tellg();
            }
            continue;
        }
        if (value == "+") {
            if (nestLevel == 0) {
                orIndex = formulaStream.tellg();
                break;
            }
            continue;
        }
        isLiteral = true;

    }
    struct formula * f = new struct formula;

    if (orIndex != -1) {
        f->value = "+";
        f->left = parse(formula.substr(0, orIndex - 1));
        f->right = parse(formula.substr(orIndex + 1, formula.npos));
    } else if (andIndex != -1) {
        f->value = ".";
        f->left = parse(formula.substr(0, andIndex - 1));
        f->right = parse(formula.substr(andIndex + 1, formula.npos));
    } else if (isLiteral) {
        f->value = value;
        f->left = NULL;
        f->right = NULL;
    }

    return f;

}

int main() {

    std::ifstream ifs("test.txt");
    std::string formula((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    struct formula * f = parse(formula);

    return 0;
}