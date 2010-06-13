#include "ILS.h"
#include "math.h"

void ILS(vector<vector<int>*> &cgroups, 
         vector<int> &rlenght, vector<int> &lot_s,
         vector<int> &lpiece, vector<int> &dpiece,
         vector<int> &leftover,vector<int> &used_rolls,
         vector<int> &rollType) {

  int maxit = ILS_MAXIT;
  vector<vector<int>*> initSol = genInitSol(rlenght,
                                            lpiece,
                                            dpiece,
                                            leftover,
                                            used_rolls,
                                            rollType);
  
  localSearchBB(initSol,rlenght,lot_s,lpiece,dpiece,
                leftover,used_rolls);
  vector<vector<int>*> S_star = initSol;
  vector<vector<int>*> S_pert;
  vector<vector<int>*> S_pert_ls;
  pair<vector<int> ,vector<vector<int>*> > pert;
  int max_pert = MAX_PERTURB;
  while (maxit > 0) {
    pert = Perturb(S_star,leftover,MAX_PERTURB);
    S_pert = pert.second;
    localSearchBB(S_pert, rlenght, lot_s,
                  lpiece, dpiece, pert.first);

    // Ojo leftoverS1 es el leftover de S_start
    // leftoverS2 es el leftover de S_pert
    // los nombres de esa variable se deben cambiar 
    // porque ahora no coinciden.

    int tleft1 = 0;
    int leftoverS1_size = leftoverS1.size();
    for(int i = 0; i < leftoverS1_size; i++) 
      sum += leftoverS1[i];
    tleft1 = sum;
    sum = 0;
    int tleft2;
    int leftoverS2_size = leftoverS2.size();
    for(int i = 0; i < leftoverS2_size; i++) 
      sum += leftoverS2[i];
    tleft2 = sum;
  
    if (tleft1 > tleft2) {
      // Update Solution
    }
    else { // Se elige con probabilidad Simulated Annealing
      double p = exp(tleft2 - tleft1 / T);
      if (p <= PROB_MARGIN) {
        // Update Solution
      }
    }
    // Hay que buscar una manera para que la perturbacion
    // sobre S_star no se vuelve a repetir
    T *= EXP_DESC; 
    max_pert--;
  }
  maxit--;
}


// bool criteria(vector<vector<int>*> s1, 
//               vector<int> leftoverS1,
//               vector<int> leftoverS2,
//               vector<vector<int>*> s2) {
//   int promS1;
//   int sum = 0;
//   int leftoverS1_size = leftoverS1.size();
//   // Promedio leftoverS1
//   for(int i = 0; i < leftoverS1_size; i++) 
//     sum += leftoverS1[i];
//   promS1 = sum / leftoverS1_size;
//   // Promedio de leftoverS2
//   sum = 0;
//   int leftoverS2_size = leftoverS2.size();
//   int promS2;
//   for(int i = 0; i < leftoverS2_size; i++) 
//     sum += leftoverS2[i];
//   promS2 = sum / leftoverS2_size;
  
//   // Comparación de leftover
//   if (promS1 > promS2) return true;
//   else return false;
// }

  
  
