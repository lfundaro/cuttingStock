#ifndef GENETIC
#define GENETIC
#include "utilities.h"
#include "Solution.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits>
#define NUM_PERTURBATIONS 15  // No poner mas de diez !
#define MOVE_PERCENTAGE 30
#define DIFF 10
#define RO 100
#define MAX_INT numeric_limits<int>::max()
using namespace std;

Solution* randomSol(Solution &initial, vector<int> &lpiece,
                    vector<int> &rlength);

vector<Solution*> genPeople(int tam, vector<int> &rlength,
                            vector<int> &lpiece,
                            vector<int> &dpiece);

pair<Solution,Solution> Cross(Solution* mother, Solution* father);

void fixSolution(Solution &son, vector<int> &dpiece, 
                 vector<int> &rlength,vector<int> &lpiece);

bool checkConstraints(vector<int> &lpiece,
                      Solution &sol, int destiny);

int notEmptyColumn(vector<int> column);

#endif
