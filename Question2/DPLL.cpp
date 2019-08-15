#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

//Use of ENUM FOR THE BCP STATUS
enum bcpState{sat, unsat, cont};

bool in_vector ( vector<int> vecIn, int item ) {

  //Input vector generated - for internal use only!!

  vector<int>::iterator iter;

  iter = find(vecIn.begin(), vecIn.end(), item);

  //If at the end then input vector triv. case return false - bottom out
  if(iter == vecIn.end()) return false;


  return true;
}

void UR( vector< vector<int> >& inStream ){
  //Logical Complexity - perform unit resolution as described in the notes for lecture

  for ( int i = 0; i < inStream.size(); i++ ) {
    //iterate over inStream size

    if ( inStream[i].size() != 1 ) break;
    bool flag = false; //Flag for unit resolution - if it is possible to remove the 'complementor' of the unit resolution process

    for( int j = 0; j < inStream.size(); j++ ){
      if(inStream[j].size() <= 1 || !in_vector(inStream[j], -inStream[i][0]) ) break;
      inStream[j].erase(find(inStream[j].begin(), inStream[j].end(), -inStream[i][0]));
      flag = true;
    }

    if (flag){
      inStream.erase(inStream.begin() + i);
      i--;
    }
  }
}

enum bcpState BCP(vector< vector<int>>& inStr, int decVar ) {
//Logical Complexity - Performs BCP Operations - Must Refer to Notes from Prof. Ganesh to examine procedure for BCP
//CONFIRM - NOT ENTIRELY SURE, BUT LOGIC DOES WORK??
  vector<vector<int>> copy = inStr;

  for ( int i = 0; i < copy.size(); i++ ) {
    if (in_vector(copy[i], decVar) && copy[i].size() > 1) {
      copy.erase(copy.begin() + i);
      //Decrement and continue
      i=i-1;
    }
    else if(in_vector(copy[i], -decVar) && copy[i].size() > 1) {
      copy[i].erase(find(copy[i].begin(), copy[i].end(), -decVar));
    }
    else if(in_vector(copy[i], -decVar)) {
      return unsat;
    }
  }
  //Now, perform UR on resulting local val
  UR(copy);
  if (copy.size()==0){
    return sat;
  }
  inStr = copy;
  return cont;
}

bool DPLL(vector<vector<int>> inStr, int dec) {
  //Simple DPLL application integration from slides from Prof. Ganesh notes
  enum bcpState res = BCP(inStr, dec);
  if (res == sat){
    return true;
  }
  else if(res == unsat){
    return false;
  }
  return DPLL(inStr, inStr[0][0]) || DPLL(inStr, -inStr[0][0]);
}

int main()
{
  string clause;
  vector<vector<int>> inStream;

  ifstream file("Input.txt");
  if( !file.is_open() ){
    cout << "Couldn't open the file" << "\n";
    return -1;
  }
  int count = 0;
  while(getline(file,clause)){
    istringstream is(clause);
    
    if(clause[0]=='c'){
      continue;
    }
    
    int j;
    char c;
    count++;
    vector<int> clause;
    while (is >> j) {
      
        clause.push_back(j);
      
      
    }

    inStream.push_back(clause);
  }

  if(count==0){
    printf("No CNF data input\n");
    return 0;
  }

  //CLOSE THE FILE!!!!!
  file.close();



/*
What you need to do MATT is write the code to properly parse the lines as specified in the assignment documentation, for DIMACs form
This is based on the DIMACs format, but read the specs on Learn

The input format is similar to Input.txt example, but would have 0s at end of every line, and so the code needs to be updated for this



 */
  bool ret = DPLL(inStream, inStream[0][0]) || DPLL(inStream, -inStream[0][0]) ;
  string ret_val = (ret) ? "SAT" : "UNSAT";
  cout << "Input formula is: " << ret_val << endl;

  return 0;
}
