#ifndef GENETIC
#define GENETIC
#include "utilities.h"
#include "Solution.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM_PERTURBATIONS 8  // No poner mas de diez !
#define MOVE_PERCENTAGE 30
using namespace std;

Solution* randomSol(Solution &initial, vector<int> &lpiece,
                    vector<int> &rlength);

#endif
