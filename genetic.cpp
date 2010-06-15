#include "genetic.h"
using namespace std;

Solution* randomSol(Solution &initial, vector<int> &lpiece,
                    vector<int> &rlength) {
  Solution* new_solution = new Solution(initial);
  int n = NUM_PERTURBATIONS;
  int origin;
  int destiny;
  int npieces;
  int take;
  int space;
  int rep=0;
  srand(time(NULL));
  for(int i = 0; i < n; i++) {
    while (true) {
      origin = rand() % initial.size;
      npieces = new_solution->cgs[origin][origin];
      take = (MOVE_PERCENTAGE*npieces) / 100;
      if (take) { // Hay suficientes piezas para mover
        while (true) {
          destiny = rand() % new_solution->size;
          if (destiny != origin) { 
            if (rlength[new_solution->rollType[destiny]] 
                < lpiece[origin]) 
              continue;
            space = take*lpiece[origin];
            // Se ejecuta movimiento
            if (new_solution->leftover[destiny] >= space) {
              new_solution->cgs[origin][origin] -= take;
              new_solution->update(origin,lpiece,rlength);
              new_solution->cgs[destiny][origin]  += take;
              new_solution->update(destiny,lpiece,rlength);
              break;
            }
          }
          else continue;
        }
        break; // Se logro hacer perturbación
      }
      else continue; //No hay suficientes piezas para mover
    }
  }
  return new_solution;
}
