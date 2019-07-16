#ifndef TSEITIN_REPLACEMENT_H
#define TSEITIN_REPLACEMENT_H

#include <list>
#include "formula.h"

std::list<struct formula *> * perform_tseitin_replacement(struct formula * f);

#endif