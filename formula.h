#ifndef FORMULA
#define FORMULA

#include <string>

struct formula {
    std::string value;
    formula * left;
    formula * right;
};
#endif