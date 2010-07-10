#include "scatterSearch.h"
using namespace std;

int diff(Solution& sol1, Solution& sol2){
  int diff = 0;
  int ngroups = min(sol1.cgs.size(),sol2.cgs.size());
  int npieces = sol1.cgs[0].size();

  for (int i = 0; i<ngroups; ++i){
    for (int j = 0; j<npieces; ++j){
      diff += abs(sol1.diversity[i][j]-sol2.diversity[i][j]);
    }
  }

  return diff;
}

// vector<int> diversity(vector< Solution >& refSet,
// 		      vector< Solution >& sols){
//   int nsols = sols.size();
//   int refSet = refSet.size();
//   int div = 0;
//   vector<int> divs = vector(nsols);

//   for (int i = 0; i<nsols; ++i){
//     for (int j = 0; j<nrefSet; ++j){
//       div += diff(sols[i],sols[j]);
//     }
//     divs[i] = div;
//     div = 0;
//   }

//   return divs;
// }

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
  int cycle = MAX_CYCLE;
  for(int i = 0; i < P_size; i++) {
    ramdSol = randomSol(initial, lpiece, rlength);
    ramdSol.fitnessEval();
    localSearchBB(ramdSol, rlength, lot_s, lpiece, dpiece);
    ramdSol.fitnessEval();
    index = linSearch(control, ramdSol.fitness);
    if (index == -1) {
      // Se agrega solución ya que no está en conjunto P
      Pset.push_back(ramdSol);
      ramdSol.printSolution();
      control.push_back(make_pair(i,ramdSol.fitness));
      sort(control.begin(), control.end(), comparePairDouble);
      cycle--;
    }
    else { // Hay un elemento que tiene el mismo fitness 
           // que la solución generada aleatoriamente. 
           // Por lo que verificamos si son verdaderamente
           // iguales.
      int u = diff(ramdSol, Pset[index]);
      // if (diff(ramdSol, Pset[index]) < 0) {
      //   // Si true entonces se descarta la solución 
      //   i--;
      //   cout << "diff" << endl;
      // }
      // else {  // No son iguales => se agrega ramdSol a Pset
      //   //        ramdSol.fitnessEval();
      //   Pset.push_back(ramdSol);
      //   ramdSol.printSolution();
      //   control.push_back(make_pair(i,ramdSol.fitness));
      //   sort(control.begin(), control.end(), comparePairDouble);
      // }

      if (cycle < 0) {cycle = MAX_CYCLE; i++;}
      else i--;
    }
  }
  return Pset;
}




