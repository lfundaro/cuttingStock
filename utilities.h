#ifndef utilities
#define utilities
#include <utility>
#include <vector>
#include <limits>
#include "tunning.h"
#define MAX_INT numeric_limits<int>::max()
using namespace std;

bool comparePair(pair<int,int>, pair<int,int>);

bool comparePairDouble(pair<int,double> a, pair<int,double> b);

bool group_quality(int *, vector<int> &,
                   vector<int> &);

int* bestCutting(vector<int>&,vector<int>&,vector<int>&);

vector<vector<int>*> genInitSol(vector<int> &, 
                                vector<int> &,
                                vector<int> &,
                                vector<int> &,
                                vector<int> &,
                                vector<int> &);


#endif
