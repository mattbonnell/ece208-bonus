#ifndef PARSE_H
#define PARSE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "formula.h"


struct formula * parse(std::string formula);
#endif