#include "scatterSearch.h"
using namespace std;

Solution scatterSearch(int P_size, int b, 
                       vector<int> &rlength,
                       vector<int> &lpiece, 
                       vector<int> &dpiece,
                       vector<int> &lot_s) {
  vector<Solution> P = genPset(rlength,lpiece,dpiece,P_size,
                               lot_s);
}


vector<Solution> genPset(vector<int> &rlength,
                         vector<int> &lpiece,
                         vector<int> &dpiece,
                         int P_size,
                         vector<int> &lot_s) {
  vector<Solution> Pset;
  Solution initial = Solution(rlength, lpiece, dpiece);
  Solution ramdSol;
  vector<pair<int,double> > control;
  Pset.reserve((size_t) P_size);
  int index;
  for(int i = 0; i < P_size; i++) {
    ramdSol = randomSol(initial, lpiece, rlength);
    localSearchBB(ramdSol, rlength, lot_s, lpiece, dpiece);
    index = linSearch(control, ramdSol.fitness);
    if (index == -1) 
      // Se agrega solución ya que no está en conjunto P
      Pset.push_back(ramdSol);
    else { // Hay un elemento que tiene el mismo fitness 
           // que la solución generada aleatoriamente. 
           // Por lo que verificamos si son verdaderamente
           // iguales.
      if (compareSolutions(ramdSol, Pset[index]))
        // Si true entonces se descarta la solución 
        i--;
      else  // No son iguales => se agrega ramdSol a Pset
        Pset.push_back(ramdSol);
    }
  }
}




