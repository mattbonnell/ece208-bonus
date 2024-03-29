
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
#include <fstream>
#include "parse.h"

//#include "C:\Users\Jason\Desktop\Summer 2019 - 2B\ECE 208\ExtraCredit\project1\treeCNF.cpp"

/*
    This code component should take the root nodes provided by (Matt) Tseitin subtrees and send to graphCNF to be converted to CNF.
    Once converted to CNF, the result must be added to a linked list which contains the subtrees all in cnf
    Finally, onnce function call to print the linked list is called, this program should print to file the current trees traversed via the linked list - top level

 */
#include "formula.h"
using namespace std;
struct Node;

// struct formula{
//     string value;
    
//     formula* left;
//     formula* right;
// };

struct Node { 
    // ONLY USE WITH LINKED LIST!
    //string value;
    Node* next;
    formula* value;
    // Node** left;
    // Node** right;
}; 

struct Node* head = NULL;   
void insert(formula* newData) { 
        // ONLY USE WITH LINKED LIST!

   //struct formula* newData = new formula;
   //newData->value=val;

   struct Node* new_node = new Node; 
   new_node->value = newData; 
   new_node->next = head; 
   head = new_node; 
} 
void display() { 
        // ONLY USE WITH LINKED LIST!

   struct Node* ptr;
   ptr = head;
   while (ptr != NULL) { 
      cout<< ptr->value->value <<" "; 
      ptr = ptr->next; 
   } 
} 

formula* copyTree(formula* F){
    //Should make a copy (subtree) given a rootNode

    //In order traversal
    if(F->left == NULL && (F->right == NULL)){
        struct formula *newNode = new struct formula;
        newNode->value = F->value;
        newNode->right = NULL;
        newNode->left = NULL;
        return newNode;
    }

    struct formula *newNode1 = new struct  formula;

    newNode1->left = copyTree(F->left);
    newNode1->value = F->value;
    newNode1->right = copyTree(F->right);

   
    return newNode1;
}


formula* impHelper(formula* F){
//Recursive simplifier for both implication, and iff, as well as double negation as always
//Note: Only leaf node will have both left and right as NULL;

//I NEED A CASE TO DEAL WITH RECURSIVE CALL ON A NULL NODE IN THIS CODE !!!!!!!!!! MUST IMPLEMENT !!!!!!! ?

    if(F->left==NULL && F->right == NULL){
        return F;
    }

    if(F->value == "->"){
        //Encoding for implication case
    
        F->value = "+"; //Set value of F to an OR
        struct formula *newLeft = new struct formula;
        newLeft->value = "-"; //Negation Value - if I am correct with the encoding ?????? 
        newLeft->left = F->left;
        newLeft->right = NULL;
        F->left = newLeft; //Set F left to be negation, with everything else following, right is unchanged
        F->left = impHelper(F->left);
        F->right = impHelper(F->right);
        return F;
    }

    if(F->value == "<->"){
        //Encoding of bidirectional case
        
        F->value = "."; //Set value to an AND
        
        //New OR Nodes
        struct formula *newLeft = new struct formula;
        struct formula *newRight = new struct formula;
        newLeft->value = "+";
        newRight->value = "+";

        //New Negation nodes
        struct formula *leftNeg = new struct formula;
        struct formula *rightNeg = new struct formula;
        leftNeg->value = "-";
        rightNeg->value = "-";

        //Assign negation children
        // leftNeg->left = F->left;
        copy_formula(leftNeg->left, F->left);
        leftNeg->right = NULL;
        // rightNeg->left = F->right;
        copy_formula(rightNeg->left, F->right);
        rightNeg->right = NULL;

        //Assign ORs connected with new negations
        newLeft->left = leftNeg;
        // newLeft->right = F->right;
        copy_formula(newLeft->right, F->right);
        newRight->left = rightNeg;
        // newRight->right = F->left;
        copy_formula(newRight->right, F->left);

        //Assign parent node to new ORs
        F->left = newLeft;
        F->right = newRight;

        //Recursive simplification on children
        F->left = impHelper(F->left);
        F->right = impHelper(F->right);
        return F;
    }

    if(F->value == "."){
        //Encoding for AND case

        F->right = impHelper(F->right);
        F->left = impHelper(F->left);
        return F;
    }

    if(F->value == "+"){
        //Encoding for OR case

        F->right = impHelper(F->right);
        F->left = impHelper(F->left);
        return F;
    }

    //Now need case for double negation, and a separate case for negation yet not a double negation
    if(F->value == "-" && (F->left->value == "-")){
        //Encoding for double negation
        struct formula *point = new struct formula;
        point = F->left->left; //Point to the first non-negation value
        delete(F->left);
        
        //Not sure about this!!! - Check
        F = point;

        F = impHelper(F);
        return F;
    }

    if(F->value == "-" && (F->left->value != "-")){
        //Encoding for single negation

        F->left = impHelper(F->left);
        return F;
    }

    return F;
}

formula* negHelper(formula* F){
//Will be called, always, after simplification of imps and bidirectional imps, so apply DeMorgans Laws now

    if(F->left==NULL && F->right == NULL){
    //Encoding of a Leaf
    return F;
    }

    if(F->value == "-" && (F->left->value == ".")){
        //Encoding for negation around an AND clause
        
        F->value = "+";
        struct formula *leftNeg = new struct formula;
        struct formula *rightNeg = new struct formula;

        // leftNeg->left = F->left->left;
        copy_formula(leftNeg->left, F->left->left);
        leftNeg->right = NULL; //Since neg node
        rightNeg->right = NULL;
        // rightNeg->left = F->left->right;
        copy_formula(rightNeg->left, F->left->right);

        F->left = leftNeg;
        F->right = rightNeg;

        F->left = negHelper(F->left);
        F->right = negHelper(F->right);
        return F;
    }

    if (F->value == "-" && (F->left->value == "+")){
        //Encoding for negation around an ORed clause

        F->value = ".";
        struct formula *leftNeg = new struct formula;
        struct formula *rightNeg = new struct formula;

        // leftNeg->left = F->left->left;
        copy_formula(leftNeg->left, F->left->left);
        // rightNeg->left = F->left->right;
        copy_formula(rightNeg->left, F->left->right);
        leftNeg->right = NULL;
        rightNeg->right = NULL;

        F->left = leftNeg;
        F->right = rightNeg;

        F->left = negHelper(F->left);
        F->right = negHelper(F->right);
        return F;
    }

    if(F->value == "-" && (F->left->value == "-")){
        //Encoding for double negation

        struct formula *point = new struct formula;
        point = F->left->left; //Point to the first non-negation value
        delete(F->left);
        
        //Not sure about this!!! - Check
        F = point;

        F = negHelper(F);
        return F;
    }

    if(F->value == "-" && (F->left->value != "-")){
        F->left = negHelper(F->left);
        return F;
    }

    if(F->value == "." || F->value == "+"){
        //End case for all else which is AND or OR alone

        F->left = negHelper(F->left);
        F->right = negHelper(F->right);
        return F;
    }

    return F;
}

formula* CNF(formula* F){
    //Should apply distribution of disjunctions over conjunctions

    if(F->left == NULL && (F->right == NULL)){
        return F; //Reached a leaf node
    }

    if(F->value == "+" && (F->left->value == ".")){
        //Case of distribution detected on left first

        F->value == ".";
        
        //Create new AND nodes
        struct formula *newLeft = new struct formula;
        struct formula *newRight = new struct formula;

        // newLeft->left = F->left->left;
        copy_formula(newLeft->left, F->left->left);
        // newLeft->right = F->right;
        copy_formula(newLeft->right, F->right);
        newLeft->value = "+";
        newRight->value = "+";

        // newRight->left = F->left->right;
        copy_formula(newRight->left, F->left->right);
        newRight->right = F->right; //ISNT;T THERE ISSUE HERE AS NOT REPEATING THE CHILDREN BUT POINTING TO SAME RESOURCES

        F->left = newLeft;
        F->right = newRight;

        F->left = CNF(F->left);
        F->right = CNF(F->right);
        return F;
    }


    if(F->value == "+" && (F->right->value == ".")){
        //Case of distribution detected on right

        F->value == ".";
        
        //Create new AND nodes
        struct formula *newLeft = new struct formula;
        struct formula *newRight = new struct formula;
        newLeft->value = "+";
        newRight->value = "+";

        // newLeft->left = F->right->left;
        copy_formula(newLeft->left, F->right->left);
        // newLeft->right = F->left;
        copy_formula(newLeft->right, F->left);

        // newRight->left = F->right->right;
        copy_formula(newRight->left, F->right->right);
        // newRight->right = F->left; //ISNT;T THERE ISSUE HERE AS NOT REPEATING THE CHILDREN BUT POINTING TO SAME RESOURCES

        F->left = newLeft;                                                                              
        F->right = newRight;

        F->left = CNF(F->left);
        F->right = CNF(F->right);
        return F;
    }

    return F;
}


//Now need to run CNF on it
formula* NNF(formula* F){
    //Takes in a pointer to a node, returns pointer to root
    //Call helper functions for implication and negation
    //Implication Helper:
    F = impHelper(F);

    //Negation helper:
    F = negHelper(F);

    return F;
    
}
int ill = 0;

void printTree(struct formula* root, ostream &outfile){  

//Write to output file


//outfile << "Test Jason" << endl;

// outfile << "a " << endl ;
// outfile << "b " << endl;


    if (root == NULL) {
        return;
    }
    // cout << "Here0";
    if(ill==0){
        // cout << "Now" << endl;
        ill=1;
        // printTree(root->left);
    
    }
    // if (root->left && root->right) {
    //     cout << root->left->value;
    //     cout << root->right->value;
    // }
    //cout << root->left->left->left->value;

    if((root->left == NULL) && (root->right == NULL)){
        // cout << "Here1";
        cout << root->value << " ";
        outfile  << root->value << " ";
        return;
    }

    if(root->value == "-"){
        // cout << "Here2";
        cout << root->value << " ";
        outfile  << root->value;
        printTree(root->left, outfile);
    }

    else{
        if(root->value == "+") {
            printTree(root->left, outfile);
            printTree(root->right, outfile);
        } else {
            printTree(root->left, outfile);
            outfile << "0\n";
            printTree(root->right, outfile);
        }

    }
    //outfile << " " << "0" << "\n";
    //outfile << "[end_of_output]" << endl;
    
    return;
}