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
#define DIFF 1
#define RO 100
#define MAX_INT numeric_limits<int>::max()
#define MAX_DOUBLE numeric_limits<double>::max()
#define MIN_DOUBLE numeric_limits<double>::min()
#define CROSS_PROB 0.2  // Para valores [0.4,1] hay cruce
#define MUTATION_FACT 0.09   // Factor de mutación
using namespace std;

Solution randomSol(Solution &initial, vector<int> &lpiece,
                    vector<int> &rlength);

vector<Solution> genPeople(int tam, vector<int> &rlength,
                            vector<int> &lpiece,
                            vector<int> &dpiece);

pair<Solution,Solution> Cross(Solution* mother, Solution* father);

void fixSolution(Solution &son, vector<int> &dpiece, 
                 vector<int> &rlength,vector<int> &lpiece);

void replace(vector<Solution> &people,
             Solution &son);


Solution get_best(vector<Solution> &people);

Solution geneticAlgorithm(int tam, vector<int> &rlength,
                           vector<int> &dpiece,
                           vector<int> &lpiece,
                           int genNum);

pair<Solution,Solution> getParents(vector<Solution> &people, vector<double> &prob);

vector<double> roulette(vector<Solution> &people);

void mutate(Solution &child,vector<int> &rlength,
            vector<int> &lpiece);

int notEmptyColumn(vector<int> column);

bool checkConstraints(vector<int> &lpiece,
                      Solution &sol, int destiny,
                      int pieceType);

void addPiece(vector<int> & targetIndex, Solution &son,
              int udiff, vector<int> &rlength, 
              vector<int> &lpiece, int pieceType);



#endif
