#ifndef UTILITIES
#define UTILITIES

#include <utility>
#include <vector>
#include <list>
#include <limits>

#include "FFD.h"

#define MAX_INT numeric_limits<int>::max()
#define MIN_INT numeric_limits<int>::min()
using namespace std;

void printCG(vector<vector<int>*>&);

void printVect(vector<int>&);

int min(int,int);

int max_item(vector<int>&);

int* nth(list<int*>*, int);

vector<vector<int>*> duplicate(vector<vector<int>*> &,int);

void overwrite(vector<vector<int>*>&,vector<vector<int>*>&,int);

void substitute(vector<vector<int>*>&,vector<vector<int>*>&,int);

bool comparePair(pair<int,int>, pair<int,int>);

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
