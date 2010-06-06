#ifndef utilities
#define utilities
#include <utility>
#include <vector>
#include <limits>
#define MAX_INT numeric_limits<int>::max()
using namespace std;

bool comparePair(pair<int,int>, pair<int,int>);

bool group_quality(vector<int>&, 
                   vector<int>&, 
                   pair<int, vector<int> >&, 
                   pair<int, vector<int> >&);

vector<vector<int>*> genInitSol(vector<int> &, 
                                vector<int> &,
                                vector<int> &,
                                vector<int> &,
                                vector<int> &);


#endif
