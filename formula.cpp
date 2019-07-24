#include "formula.h"

void copy_formula(formula *&dest, formula *src) {
    if (!src) {
        return;
    }
    dest = new formula;
    dest->value = src->value;
    copy_formula(dest->left, src->left);
    copy_formula(dest->right, src->right);
}