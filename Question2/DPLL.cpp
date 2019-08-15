#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;
//Completed August 15th check 12:31:32 PM

enum STATE{sat, unsat, contingent};
enum STATE BCP(vector<vector<int>>& F, int prop);


bool iterate (vector<int> vec, int item){
  //As follow from simple example, simply iterates over a vector and determines if item is inside it
  vector<int>::iterator trav;

  trav = find(vec.begin(),vec.end(), item);

  if(trav==vec.end()){
    return false;
  }
  else{
    return true;
  }
}

//Unit Resolution - to Be Performed 
//Follow Prof. Ganesh's slides from lecture on this very closely!!!!
void unitResolution(vector<vector<int>>& F){
  for (int i = 0; i < F.size(); i++) {
    if (F[i].size() != 1){
      //No Unit Resolution Possible for this clause obviously!
      break;
    }
    bool mark = false;
    //Iterate over the formula now and look for prop var and its negation within outer forloop
    for(int j = 0; j < F.size(); j++){
      if(F[j].size() <= 1 || !iterate(F[j], -F[i][0])){
        break;
      }
      //Erase the prop var and clause with single unit if applicable (no break) and set mark to true
      F[j].erase(find(F[j].begin(), F[j].end(), -F[i][0]));
      mark = true;
    }
    if (mark){
      //Clause Erasor
      F.erase(F.begin() + i);
      i--;
    }
  }
}

bool DPLL(vector<vector<int>> formula, int A) {
  //Follow from Slide - basic DPLL execution of algorithm
  // A - variable assignment
  //INput to BCP, get the output
  enum STATE res = BCP(formula, A);
  if (res == sat){
    return true;
  }
  else if (res == unsat) {
  return false;
  }
  //Call DPLL on both variable true and its negation, false as indicated by prop. variable value as either + or - below
  return DPLL(formula, formula[0][0]) || DPLL(formula, -formula[0][0]);
}

// BCP Algorithm - More Complicated !!!
enum STATE BCP(vector<vector<int>>& F, int prop) {
  //Create a soft-copy of formula passed by adress to iterate over and perform contraint propgation
  vector<vector<int>> sF = F;
  for (int j=0; j<sF.size(); j++){
    //Iterating over the input formula clauses given a propositional variable, if it exists in both forms, erase it from its index
    if (iterate(sF[j], prop)) {
      //Erase macro
      sF.erase(sF.begin() + j);
      j--;
    }
    //Check now in other case if negation of propositional var resent
    else if(iterate(sF[j], -prop) && sF[j].size() > 1) {
      //Erase occurences of the negation of the propositional variable within the bounds of the input clause
      sF[j].erase(find(sF[j].begin(), sF[j].end(), -prop));
    }
    else if(iterate(sF[j], -prop)) {
      //If iterated over and no found, then unsat return
      return unsat;
    }
  }

  //After changes made perform unit resolution on soft copy formula and of course, assign original formula to softcopy
  unitResolution(sF);
  F = sF;
  if (sF.size()==0){ 
    //triv case if simplified all vars out
    return sat;
  }

  return contingent;
}

int main(int argc, char* argv[]){
  //Generate a vector of vectored ints for the formula internal representation
  //Generate input string below from the cnf file for our use
  vector<vector<int>> F;
  string inString;

  //Standard ifstream which will bve used to create a stringstream????
  //DOES BELOW WORK - - - - - NEED TO TEST??????????????????????
  ifstream file(argv[1]);

  if(!file.is_open()){
    cout << "Error In Opening File. Please check your input a try again.";
    fprintf(stderr, "Error in Opening File");
    return -1;
  }

  //Process of Parsing the Input String and populating formula for our internal use below
  while(getline(file,inString)){

    //Get Input String Stream 
    istringstream streamIn(inString);
    int valid;

    //Vector for the line delimited clause in the CNF input format file
    vector<int> clause;

    //Get next character in input sequence - first time use 
    char var=streamIn.peek();

    if(var== 'c' || var=='p'){
      //CNF file comment -
      continue;
    }

    while (streamIn >> valid && valid != 0) {
      clause.push_back(valid);
    }

    F.push_back(clause);
  }
  file.close();

  //Launch the DPLL algorithm from here on both prop var initial and its negation assignment recursively
  bool ret = DPLL(F, F[0][0]) || DPLL(F, -F[0][0]);
  string result = (ret) ? "SAT" : "UNSAT";

  cout << "The formula, as determined by this SAT Solver is: " << result << endl;

  return 0;
}
