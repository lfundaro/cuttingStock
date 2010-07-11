#ifndef UTILITIES_H
#define UTILITIES_H
#include <utility>
#include <vector>
#include <limits>
#include <math.h>
#include "tunning.h"
#include "FFD.h"
#include "Solution.h"
#define MAX_INT numeric_limits<int>::max()
using namespace std;

bool comparePair(pair<int,int>, pair<int,int>);

bool comparePairDouble(pair<int,double>,pair<int,double>);

bool group_quality(int *, vector<int> &,
                   vector<int> &);

int* bestCutting(vector<int>&,vector<int>&,vector<int>&);

vector<vector<int>*> genInitSol(vector<int> &, 
                                vector<int> &,
                                vector<int> &,
                                vector<int> &,
                                vector<int> &,
                                vector<int> &);


int* bestCutting(vector<int>& pieceSet, 
                 vector<int>& rlength,
                 vector<int>& lpiece);

int notEmptyColumn(vector<int> column);

vector<int> calcVariety(vector<vector<int> > &cgs);

vector<vector<int> > calcDiversity(int rolltypes, 
                                   vector<vector<int> > &cgs,
                                   vector<int> &rollType,
                                   vector<int> &used_rolls);

int linSearch(vector<pair<int,double> > control, 
              double target);

int* twoOnN(int[],int[],int);



double variance(vector<int>&);

double cg_standarDev(vector< vector<int> > &);

void printCG(vector< vector<int> > &);

#endif
