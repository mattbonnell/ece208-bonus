// Component of Tseitin Transformer - Jason Antao
//First need to define structure for my node which I will be dealing with

#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<string.h>

using namespace std;

//Node Structure
struct Node{
// Value, pointers to two node children
string value;
Node* leftChild;
Node* rightChild;
};

struct lNode{
    Node* node;
    Node* next;
};

//Now, need function to convert to NNF first, as that is the major work - converting to NNF

Node* NNF(Node* F){
    //Takes in a pointer to a node, returns pointer to root
    //Call helper functions for implication and negation
    //Implication Helper:
    F = impHelper(F);

    //Negation helper:
    F = negHelper(F);

    return F;
}

Node* impHelper(Node* F){
//Recursive simplifier for both implication, and iff, as well as double negation as always
//Note: Only leaf node will have both leftChild and rightChild as NULL;

//I NEED A CASE TO DEAL WITH RECURSIVE CALL ON A NULL NODE IN THIS CODE !!!!!!!!!! MUST IMPLEMENT !!!!!!!

    if(F->leftChild==NULL && F->rightChild == NULL){
        return F;
    }

    if(F->value == "->"){
        //Encoding for implication case

        F->value = "+"; //Set value of F to an OR
        struct Node *newLeft = new struct Node;
        newLeft->value = "-"; //Negation Value - if I am correct with the encoding ?????? 
        newLeft->leftChild = F->leftChild;
        newLeft->rightChild = NULL;
        F->leftChild = newLeft; //Set F leftChild to be negation, with everything else following, rightChild is unchanged
        F->leftChild = impHelper(F->leftChild);
        F->rightChild = impHelper(F->rightChild);
    }

    if(F->value == "<->"){
        //Encoding of bidirectional case
        
        F->value = "."; //Set value to an AND
        
        //New OR Nodes
        struct Node *newLeft = new struct Node;
        struct Node *newRight = new struct Node;
        newLeft->value = "+";
        newRight->value = "+";

        //New Negation nodes
        struct Node *leftNeg = new struct Node;
        struct Node *rightNeg = new struct Node;
        leftNeg->value = "-";
        rightNeg->value = "-";

        //Assign negation children
        leftNeg->leftChild = copyTree(F->leftChild);
        leftNeg->rightChild = NULL;
        rightNeg->leftChild = copyTree(F->rightChild);
        rightNeg->rightChild = NULL;

        //Assign ORs connected with new negations
        newLeft->leftChild = leftNeg;
        newLeft->rightChild = F->rightChild;
        newRight->leftChild = rightNeg;
        newRight->rightChild = F->leftChild;

        //Assign parent node to new ORs
        F->leftChild = newLeft;
        F->rightChild = newRight;

        //Recursive simplification on children
        F->leftChild = impHelper(F->leftChild);
        F->rightChild = impHelper(F->rightChild);
    }

    if(F->value == "."){
        //Encoding for AND case

        F->rightChild = impHelper(F->rightChild);
        F->leftChild = impHelper(F->leftChild);
    }

    if(F->value == "+"){
        //Encoding for OR case

        F->rightChild = impHelper(F->rightChild);
        F->leftChild = impHelper(F->leftChild);
    }

    //Now need case for double negation, and a separate case for negation yet not a double negation
    if(F->value == "-" && (F->leftChild->value == "-")){
        //Encoding for double negation
        struct Node *point = new struct Node;
        point = F->leftChild->leftChild; //Point to the first non-negation value
        delete(F->leftChild);
        
        //Not sure about this!!! - Check
        F = point;

        F = impHelper(F);
    }

    if(F->value == "-" && (F->leftChild->value != "-")){
        //Encoding for single negation

        F->leftChild = impHelper(F->leftChild);
    }

    return F;
}

Node* negHelper(Node* F){
//Will be called, always, after simplification of imps and bidirectional imps, so apply DeMorgans Laws now

    if(F->leftChild==NULL && F->rightChild == NULL){
    //Encoding of a Leaf
    return F;
    }

    if(F->value == "-" && (F->leftChild->value == ".")){
        //Encoding for negation around an AND clause
        
        F->value = "+";
        struct Node *leftNeg = new struct Node;
        struct Node *rightNeg = new struct Node;

        leftNeg->leftChild = F->leftChild->leftChild;
        leftNeg->rightChild = NULL; //Since neg node
        rightNeg->rightChild = NULL;
        rightNeg->leftChild = F->leftChild->rightChild;

        F->leftChild = leftNeg;
        F->rightChild = rightNeg;

        F->leftChild = negHelper(F->leftChild);
        F->rightChild = negHelper(F->rightChild);
    }

    if (F->value == "-" && (F->leftChild->value == "+")){
        //Encoding for negation around an ORed clause

        F->value = ".";
        struct Node *leftNeg = new struct Node;
        struct Node *rightNeg = new struct Node;

        leftNeg->leftChild = F->leftChild->leftChild;
        rightNeg->leftChild = F->leftChild->rightChild;
        leftNeg->rightChild = NULL;
        rightNeg->rightChild = NULL;

        F->leftChild = leftNeg;
        F->rightChild = rightNeg;

        F->leftChild = negHelper(F->leftChild);
        F->rightChild = negHelper(F->rightChild);
    }

    if(F->value == "-" && (F->leftChild->value == "-")){
        //Encoding for double negation

        struct Node *point = new struct Node;
        point = F->leftChild->leftChild; //Point to the first non-negation value
        delete(F->leftChild);
        
        //Not sure about this!!! - Check
        F = point;

        F = negHelper(F);
    }

    if(F->value == "-" && (F->leftChild->value != "-")){
        F->leftChild = negHelper(F->leftChild);
    }

    if(F->value == "." || F->value == "+"){
        //End case for all else which is AND or OR alone

        F->leftChild = negHelper(F->leftChild);
        F->rightChild = negHelper(F->rightChild);
    }

    return F;
}

//Now has gone through once called with NNF function, and should have tree in NNF, but now need to convert the NNF to CNF
//Note that this method also has it simplified to at most two children per node

Node* CNF(Node* F){
    //Should apply distribution of disjunctions over conjunctions

    if(F->leftChild == NULL && (F->rightChild == NULL)){
        return F; //Reached a leaf node
    }

    if(F->value == "+" && (F->leftChild->value == ".")){
        //Case of distribution detected on left first

        F->value == ".";
        
        //Create new AND nodes
        struct Node *newLeft = new struct Node;
        struct Node *newRight = new struct Node;

        newLeft->leftChild = F->leftChild->leftChild;
        newLeft->rightChild = F->rightChild;
        newLeft->value = "+";
        newRight->value = "+";

        newRight->leftChild = F->leftChild->rightChild;
        newRight->rightChild = copyTree(F->rightChild); //ISNT;T THERE ISSUE HERE AS NOT REPEATING THE CHILDREN BUT POINTING TO SAME RESOURCES

        F->leftChild = newLeft;
        F->rightChild = newRight;

        F->leftChild = CNF(F->leftChild);
        F->rightChild = CNF(F->rightChild);
    }


    if(F->value == "+" && (F->rightChild->value == ".")){
        //Case of distribution detected on right

        F->value == ".";
        
        //Create new AND nodes
        struct Node *newLeft = new struct Node;
        struct Node *newRight = new struct Node;
        newLeft->value = "+";
        newRight->value = "+";

        newLeft->leftChild = F->rightChild->leftChild;
        newLeft->rightChild = F->leftChild;

        newRight->leftChild = F->rightChild->rightChild;
        newRight->rightChild = copyTree(F->leftChild); //ISNT;T THERE ISSUE HERE AS NOT REPEATING THE CHILDREN BUT POINTING TO SAME RESOURCES

        F->leftChild = newLeft;
        F->rightChild = newRight;

        F->leftChild = CNF(F->leftChild);
        F->rightChild = CNF(F->rightChild);
    }

    return F;
}


Node* copyTree(Node* F){
    //Should make a copy (subtree) given a rootNode

    //In order traversal
    if(F->leftChild == NULL && (F->rightChild == NULL)){
        struct Node *newNode = new struct Node;
        newNode->value = F->value;
        newNode->rightChild = NULL;
        newNode->leftChild = NULL;
        return newNode;
    }

    struct Node *newNode1 = new struct  Node;

    newNode1->leftChild = copyTree(F->leftChild);
    newNode1->value = F->value;
    newNode1->rightChild = copyTree(F->rightChild);

   
    return newNode1;
}

Node* tseitinStep(Node* F, int i){
    //Passed in a node without new variable - then create new variable and link to F, then convert to CNF and then place into linked list

    //Need to construct nodes for Tsetin to use of size three

    

    struct Node *newVar = new struct Node;
    newVar->value = to_string(i); //Set new variable introduced

    newVar->leftChild = NULL;
    newVar->rightChild = NULL;

    struct Node *iff = new struct Node;
    iff->value = "<->";
    iff->leftChild = newVar;
    iff->rightChild = F;

    iff = NNF(iff);
    iff = CNF(iff);

    //This returns the G(X,Y) relationship for a given subtree
    return iff;
}


//Now, to link up, need to go through three and reassign 'F' in tseitingStep above to ONLY be comprised of three nodes, where the root node F 