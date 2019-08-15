#include "parse.h"
#include "tseitin_replacement.h"
#include "treeCNF.h"
#include <fstream>

int main() {
    std::ifstream ifs("test.txt");
    std::string formula_string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    formula *f = parse(formula_string);
    std::list<formula *> *sub_formulas = perform_tseitin_replacement(f);
    std::ofstream outfile("outputFormula.txt");
    for(std::list<formula *>::iterator it = sub_formulas->begin(); it != sub_formulas->end(); ++it) {
        printTree((formula *)*it, outfile);
        formula *converted = NNF((formula *)*it);
        converted = CNF(converted);
        printTree(converted, outfile);
        outfile << "0\n";
    }
    //test point
    delete f;
    delete sub_formulas;
    return 0;
}
