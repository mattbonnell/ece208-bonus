#ifndef FORMULA_H
#define FORMULA_H

#include <string>

struct formula {
    std::string value;
    formula * left;
    formula * right;
};

void copy_formula(formula *&dest, formula *src);
#endif