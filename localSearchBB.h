#ifndef LOCALSEARCHBB
#define LOCALSEARCHBB

#include <vector>

#include "shiftSpace.h"

#define LSBB_MAXIT 2000 // Criterio de parada

using namespace std;

void localSearchBB(vector<vector<int>*> &, 
                   vector<int> &, vector<int> &,
                   vector<int> &, vector<int> &,
                   vector<int> &, vector<int> &,
		   vector<int> &);

#endif

