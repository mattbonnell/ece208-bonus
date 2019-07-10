#include <string>
#ifndef FORMULA
#define FORMULA
struct formula {
    std::string value;
    formula * left;
    formula * right;
};
#endif