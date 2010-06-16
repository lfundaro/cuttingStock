//#ifndef ILS
//#define ILS

#include "math.h"

#include "localSearchBB.h"
#include "utilities.h"
#include "Perturb.h"

#define MAX_PERTURB 4
#define ILS_MAXIT 1000  // Criterio de parada
#define INIT_TEMP 500
#define EXP_DESC 0.99   // Descenso exponencial

void ILS(vector<vector<int>*> &, 
         vector<int> &,vector<int> &,
         vector<int> &,vector<int> &,
         vector<int> &,vector<int> &,
         vector<int> &,vector<int> &);

//#endif
