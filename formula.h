#include <string>

struct formula {
    std::string value;
    formula * left;
    formula * right;
};