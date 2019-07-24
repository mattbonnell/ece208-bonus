#ifndef FORMULA_H
#define FORMULA_H

#include <string>

struct formula {
    std::string value;
    formula * left;
    formula * right;
};

void copy(formula *&dest, formula *src) {
    if (!src) {
        return;
    }
    dest = new formula;
    dest->value = src->value;
    copy(dest->left, src->left);
    copy(dest->right, src->right);
}
#endif