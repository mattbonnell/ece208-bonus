#include "formula.h"
#include <list>
#include "parse.h"


void _perform_tseitin_replacement_recursive(formula * f, std::list<formula *> * sub_formulas) {
    if (f->value == "+" || f->value == ".") {
        _perform_tseitin_replacement_recursive(f->left, sub_formulas);
        _perform_tseitin_replacement_recursive(f->right, sub_formulas);

    } else if (f->value != "-") {
        return;
    }
    formula * sub = new formula;
    std::string sub_var = "p" + std::to_string(sub_formulas->size());
    sub->value = "<->";
    sub->left = new formula;
    sub->left->value = sub_var;
    sub->right = f;
    sub_formulas->push_back(sub);
    f = new formula;
    f->value = sub_var;
    return;
}

std::list<formula *> * perform_tseitin_replacement(formula * f) {
    std::list<formula *> * sub_formulas = new std::list<formula *>;
    _perform_tseitin_replacement_recursive(f, sub_formulas);
    return sub_formulas;

}

int main() {
    std::ifstream ifs("test.txt");
    std::string formula_string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    formula * f = parse(formula_string);
    std::list<formula *> * sub_formulas = perform_tseitin_replacement(f);

    return 0;
}