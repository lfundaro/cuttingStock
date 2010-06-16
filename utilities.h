#ifndef UTILITIES
#define UTILITIES

#include <utility>
#include <vector>
#include <limits>
#define MAX_INT numeric_limits<int>::max()
using namespace std;

int min(int,int);

vector<vector<int>*> duplicate(vector<vector<int>*> &,int);

void overwrite(vector<vector<int>*>&,vector<vector<int>*>&,int);

bool comparePair(pair<int,int>, pair<int,int>);

bool group_quality(int *, vector<int> &,
                   vector<int> &);


vector<vector<int>*> genInitSol(vector<int> &, 
                                vector<int> &,
                                vector<int> &,
                                vector<int> &,
                                vector<int> &,
                                vector<int> &);
#endif
