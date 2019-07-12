
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<string.h>

//#include "C:\Users\Jason\Desktop\Summer 2019 - 2B\ECE 208\ExtraCredit\project1\treeCNF.cpp"

using namespace std;

/*
    This code component should take the root nodes provided by (Matt) Tseitin subtrees and send to graphCNF to be converted to CNF.
    Once converted to CNF, the result must be added to a linked list which contains the subtrees all in cnf
    Finally, onnce function call to print the linked list is called, this program should print to file the current trees traversed via the linked list - top level

 */

// struct Node{
// // Value, pointers to two node children
// string value;
// Node* leftChild;
// Node* rightChild;
// };

// struct linkedNode{
//     Node** node; //Pointer to a node
//     linkedNode* next;
// };

// int listCount=0;
// struct linkedNode* head = NULL;

// //struct Node* head = NULL;

// //Phase 1 - Place arbitrary root of subtree into linked list
// //Phase 2 - Run CNF converter on entire linked list
// //Phase 3 - Print entire linked list and all nested subtrees

// // linkedList* createList(struct Node* root){
// //     struct linkedList *treeList = new struct linkedList;
// //     treeList->head = root;
// //     treeList->tail = root;
// //     treeList->nextNode = NULL;
// //     length=1;

// //     return treeList;
// // }

// void insertTree(struct Node* newRoot){

//     if(listCount > 0){
//     cout << "Head Value " <<(*((head)->node))->value << endl;
//     }
//     //Phase 1
//     struct linkedNode* newNodeValue = new struct linkedNode;
//     newNodeValue->node = &newRoot;

//     cout << "Inserting " << newRoot->value << endl;


//     newNodeValue->next = head;

//     head=newNodeValue;

//     if(listCount>0){
//         cout << "Next node: " << (*((head->next)->node))->value << endl;
//     }
//     cout << "Value of Node Inserted is " << (*((head)->node))->value << endl;
//     listCount++;
// }

// void displayList(){
//     struct linkedNode *ptr;
//     ptr = head;
//     while(ptr!=NULL){
//         cout << (*(ptr->node))->value << endl;
//         //cout << " " << endl;
//         ptr = ptr->next;
//     }

// }

// int main (int argc, char *argv[]){
//    //Create the nodes

//    struct Node* first = new struct Node;
//    first->value="first";
//    insertTree((first));


//  cout << "Main Head Value " <<(*((head)->node))->value << endl;
//   // displayList();

//    struct Node test;
//    cout << "Main Head Value " <<(*((head)->node))->value << endl;
//    test.value="Test";
//    cout << "Main Head Value " <<(*((head)->node))->value << endl;
//    insertTree((&test));

//    displayList();
   
// //    struct Node* third = new struct Node;
// //    third->value="third";
// //    insertTree((third));

// //    displayList();

// //    struct Node* fourth = new struct Node;
// //    fourth->value="fourth";
// //    insertTree((fourth));

// //    displayList();

// //    struct Node* fifth = new struct Node;
// //    fifth->value="fifth";
// //    insertTree((fifth));

// //    displayList();

//    return 0;

// }




#include <iostream>
#include <string.h>
using namespace std;
struct Node;

struct dataNode{
    string value;
    
    dataNode* leftChild;
    dataNode* rightChild;
};

struct Node { 
    //string value;
    Node* next;
    dataNode* value;
    // Node** leftChild;
    // Node** rightChild;
}; 

struct Node* head = NULL;   
void insert(dataNode* newData) { 
   //struct dataNode* newData = new dataNode;
   //newData->value=val;

   struct Node* new_node = new Node; 
   new_node->value = newData; 
   new_node->next = head; 
   head = new_node; 
} 
void display() { 
   struct Node* ptr;
   ptr = head;
   while (ptr != NULL) { 
      cout<< ptr->value->value <<" "; 
      ptr = ptr->next; 
   } 
} 

dataNode* copyTree(dataNode* F){
    //Should make a copy (subtree) given a rootNode

    //In order traversal
    if(F->leftChild == NULL && (F->rightChild == NULL)){
        struct dataNode *newNode = new struct dataNode;
        newNode->value = F->value;
        newNode->rightChild = NULL;
        newNode->leftChild = NULL;
        return newNode;
    }

    struct dataNode *newNode1 = new struct  dataNode;

    newNode1->leftChild = copyTree(F->leftChild);
    newNode1->value = F->value;
    newNode1->rightChild = copyTree(F->rightChild);

   
    return newNode1;
}


dataNode* impHelper(dataNode* F){
//Recursive simplifier for both implication, and iff, as well as double negation as always
//Note: Only leaf node will have both leftChild and rightChild as NULL;

//I NEED A CASE TO DEAL WITH RECURSIVE CALL ON A NULL NODE IN THIS CODE !!!!!!!!!! MUST IMPLEMENT !!!!!!!

    if(F->leftChild==NULL && F->rightChild == NULL){
        return F;
    }

    if(F->value == "->"){
        //Encoding for implication case
    
        F->value = "+"; //Set value of F to an OR
        struct dataNode *newLeft = new struct dataNode;
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
        struct dataNode *newLeft = new struct dataNode;
        struct dataNode *newRight = new struct dataNode;
        newLeft->value = "+";
        newRight->value = "+";

        //New Negation nodes
        struct dataNode *leftNeg = new struct dataNode;
        struct dataNode *rightNeg = new struct dataNode;
        leftNeg->value = "-";
        rightNeg->value = "-";

        //Assign negation children
        leftNeg->leftChild = F->leftChild;
        leftNeg->rightChild = NULL;
        rightNeg->leftChild = F->rightChild;
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
        struct dataNode *point = new struct dataNode;
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

dataNode* negHelper(dataNode* F){
//Will be called, always, after simplification of imps and bidirectional imps, so apply DeMorgans Laws now

    if(F->leftChild==NULL && F->rightChild == NULL){
    //Encoding of a Leaf
    return F;
    }

    if(F->value == "-" && (F->leftChild->value == ".")){
        //Encoding for negation around an AND clause
        
        F->value = "+";
        struct dataNode *leftNeg = new struct dataNode;
        struct dataNode *rightNeg = new struct dataNode;

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
        struct dataNode *leftNeg = new struct dataNode;
        struct dataNode *rightNeg = new struct dataNode;

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

        struct dataNode *point = new struct dataNode;
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

dataNode* CNF(dataNode* F){
    //Should apply distribution of disjunctions over conjunctions

    if(F->leftChild == NULL && (F->rightChild == NULL)){
        return F; //Reached a leaf node
    }

    if(F->value == "+" && (F->leftChild->value == ".")){
        //Case of distribution detected on left first

        F->value == ".";
        
        //Create new AND nodes
        struct dataNode *newLeft = new struct dataNode;
        struct dataNode *newRight = new struct dataNode;

        newLeft->leftChild = F->leftChild->leftChild;
        newLeft->rightChild = F->rightChild;
        newLeft->value = "+";
        newRight->value = "+";

        newRight->leftChild = F->leftChild->rightChild;
        newRight->rightChild = F->rightChild; //ISNT;T THERE ISSUE HERE AS NOT REPEATING THE CHILDREN BUT POINTING TO SAME RESOURCES

        F->leftChild = newLeft;
        F->rightChild = newRight;

        F->leftChild = CNF(F->leftChild);
        F->rightChild = CNF(F->rightChild);
    }


    if(F->value == "+" && (F->rightChild->value == ".")){
        //Case of distribution detected on right

        F->value == ".";
        
        //Create new AND nodes
        struct dataNode *newLeft = new struct dataNode;
        struct dataNode *newRight = new struct dataNode;
        newLeft->value = "+";
        newRight->value = "+";

        newLeft->leftChild = F->rightChild->leftChild;
        newLeft->rightChild = F->leftChild;

        newRight->leftChild = F->rightChild->rightChild;
        newRight->rightChild = F->leftChild; //ISNT;T THERE ISSUE HERE AS NOT REPEATING THE CHILDREN BUT POINTING TO SAME RESOURCES

        F->leftChild = newLeft;
        F->rightChild = newRight;

        F->leftChild = CNF(F->leftChild);
        F->rightChild = CNF(F->rightChild);
    }

    return F;
}


//Now need to run CNF on it
dataNode* NNF(dataNode* F){
    //Takes in a pointer to a node, returns pointer to root
    //Call helper functions for implication and negation
    //Implication Helper:
    F = impHelper(F);

    //Negation helper:
    F = negHelper(F);

    return F;
    
}
int ill = 0;

void printTree(struct dataNode* root){
    cout << "Here0";
    if(ill==0){
        cout << "Now" << endl;
        ill=1;
        // printTree(root->leftChild);
    
    }
    // if (root->leftChild && root->rightChild) {
    //     cout << root->leftChild->value;
    //     cout << root->rightChild->value;
    // }
    //cout << root->leftChild->leftChild->leftChild->value;

    if((root->leftChild == NULL) && (root->rightChild == NULL)){
        cout << "Here1";
        cout << root->value << " ";
        return;
    }

    if(root->value == "-"){
        cout << "Here2";
        cout << root->value << " ";
        printTree(root->leftChild);
    }

    else{
        cout << "Here3";
        printTree(root->leftChild);
        cout << " " << root->value << " ";
        printTree(root->rightChild);

    }

    return;
}
















int main() { 
    
    //Constructing a single subtree
        //Building a test Case
        struct dataNode* newData1 = new dataNode;
        struct dataNode* newData1s1 = new dataNode;
        struct dataNode* newData1s2 = new dataNode;
        struct dataNode* newData1s3 = new dataNode;
        struct dataNode* newData1s4 = new dataNode;
        
        newData1s1->value = "1";
        newData1s1->leftChild = NULL;
        newData1s1->rightChild = NULL;
        
        newData1s2->value = "2";
        newData1s2->leftChild = NULL;
        newData1s2->rightChild = NULL;
        
        newData1s3->value = "3";
        newData1s3->leftChild = NULL;
        newData1s3->rightChild = NULL;
        
        newData1s4->value = ".";
        newData1s4->leftChild = newData1s2;
        newData1s4->rightChild = newData1s3;
        
        newData1->value="<->";
        newData1->leftChild=newData1s1;
        newData1->rightChild=newData1s4;
    //End of subtree construction for one subtree


    newData1 = NNF(newData1);
    newData1 = CNF(newData1);
    printTree(newData1);

    insert(newData1);

    // struct dataNode* newData2 = new dataNode;
    // newData2->value="4";
    // insert(newData2);

    // struct dataNode* newData3 = new dataNode;
    // newData3->value="5";
    // insert(newData3);

    // struct dataNode* newData4 = new dataNode;
    // newData4->value="6";
    // insert(newData4);




   cout<<"The linked list is: ";
   display(); 
   return 0; 
} 