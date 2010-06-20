#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <iostream>

#include "utilities.h"
#include "shiftSpace.h"

#define RCL_SIZE 3

using namespace std;

vector<vector<int>*> rand_rst(vector<int>,
			      vector<int>&,
			      vector<int>&,
			      vector<int>&,
			      vector<int>&,
			      vector<int>&,
			      vector<int>&);

int* pick(int&,vector<int>&,vector<int>&,vector<int>&,int,int);

bool space_left(int,vector<int>&,vector<int>&,vector<int>&,int);

bool pieces_left(vector<int>&);

bool compareLeftovers(int*,int*);
