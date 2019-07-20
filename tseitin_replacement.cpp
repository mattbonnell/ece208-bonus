#include "formula.h"
#include <list>
#include "parse.h"
#include "treeCNF.h"


void _perform_tseitin_replacement_recursive(struct formula * f, std::list<struct formula *> * sub_formulas) {
    if (f->value == "+" || f->value == ".") {
        _perform_tseitin_replacement_recursive(f->left, sub_formulas);
        _perform_tseitin_replacement_recursive(f->right, sub_formulas);

    } else if (f->value != "-") {
        return;
    }
    struct formula * sub = new struct formula;
    std::string sub_var = "p" + std::to_string(sub_formulas->size());
    sub->value = "<->";
    sub->left = new struct formula;
    sub->left->value = sub_var;
    sub->right = f;
    sub_formulas->push_back(sub);
    printTree(sub)
    f->value = sub_var;
    // if (f->left) {
    //     delete f->left;
    // }
    f->left = NULL;
    f->right = NULL;
    return;
}

std::list<struct formula *> * perform_tseitin_replacement(struct formula * f) {
    std::list<struct formula *> * sub_formulas = new std::list<struct formula *>;
    _perform_tseitin_replacement_recursive(f, sub_formulas);
    return sub_formulas;

}