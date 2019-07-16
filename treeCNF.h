
#ifndef TREECNF_H
#define TREECNF_H

#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
#include "formula.h"

formula *NNF(formula *F);
formula *CNF(formula *F);
void printTree(formula *root);
#endif
