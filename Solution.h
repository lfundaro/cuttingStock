#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include <list>
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
  vector<int> variety;
  vector<vector<int> > diversity;
  int size;
  int penalty;
  double fitness;
  bool label;

  Solution(vector<int> &rlength, vector<int> &lpiece,
           vector<int> &dpiece);
  
  Solution(const Solution& a);

  Solution(vector<int> leftover, vector<int> used_rolls,
           vector<int> rollType, vector<vector<int> > cgs,
           double fitn, int size, int penalty,
           vector<int> variet, vector<vector<int> > divers,
           bool labl);

  Solution();

  ~Solution();

  void printSolution();

  /* void calcVariety(); */

  /* void calcDiversity(int rolltypes); */

  void update(int where, vector<int> &lpiece,
              vector<int> &rlength);

  void fitnessEval();

  void printAsPaper(vector<int> &rlength,
                              vector<int> &lpiece);
};

vector<int> calcVariety(vector<vector<int> > &cgs);

vector<vector<int> > calcDiversity(int rolltypes,
                                   vector<vector<int> > &cgs,
                                   vector<int> &rollType,
                                   vector<int> &used_rolls) ;


#endif
