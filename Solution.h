#ifndef SOLUTION
#define SOLUTION
#include <vector>
#include "utilities.h"
#include "FFD.h"
#define PENALTY 40   // Importancia de Penalty
#define LEFTOVER 60  // Importancia de Leftover
using namespace std;

class Solution {
 public:
  vector<int> leftover;
  vector<int> used_rolls;
  vector<int> rollType;
  vector<vector<int> > cgs; // cutting group set
  int size;
  int penalty;
  double fitness;

  Solution(vector<int> &rlength, vector<int> &lpiece,
           vector<int> &dpiece);
  
  Solution(const Solution& a, int M);

  Solution(vector<int> leftover, vector<int> used_rolls,
           vector<int> rollType, vector<vector<int> > cgs,
           double fitn,int size, int penalty);

  Solution();
  ~Solution();
  void printSolution();

  void update(int where, vector<int> &lpiece,
              vector<int> &rlength);

  void fitnessEval();
  //  void printSolution() {

};

#endif
