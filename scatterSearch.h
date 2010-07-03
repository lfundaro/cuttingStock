#ifndef SCATTERSEARCH
#define SCATTERSEARCH
#include "utilities.h"
#include "Solution.h"
#include "genetic.h"
using namespace std;

Solution scatterSearch(int P_size, int b, vector<int> &rlength,
                       vector<int> &lpiece, 
                       vector<int> &dpiece);

vector<Solution> genPset(vector<int> &rlength,
                         vector<int> &lpiece,
                         vector<int> &dpiece,
                         int P_size);

#endif
