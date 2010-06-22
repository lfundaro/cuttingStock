#ifndef SHIFTSPACE
#define SHIFTSPACE

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>

#include "utilities.h"
#include "FFD.h"

#define MAX_OS 4
#define MIN_SD 50
#define COMB_OP +
#define OUTSIDE_FUNC
#define DST_LO_MOD 1
#define DST_FRG_MOD 1
#define ORIG_LO_MOD 1
#define ORIG_FRG_MOD 1

using namespace std;

bool compareScores(double*,double*);

bool checkMove(int,int,int,vector<vector<int>*>*,
	       vector<int>*,vector<int>*);

int initScoreArrays(vector<vector<int>*>*,vector<int>*,
		    vector<int>*,vector<int>*,int,
		    vector<double*>*,vector<double*>*);

int* next_move(vector<int>*, int,vector<vector<int>*>*,
	       vector<int>*,vector<int>*,vector<int>*,
	       vector<int>*,vector<double*>*,vector<double*>*);
#endif
