#include "genetic.h"
using namespace std;

Solution* randomSol(Solution &initial, vector<int> &lpiece) {
  Solution* new_solution = new Solution(initial);
  int n = NUM_PERTURBATIONS;
  int origin;
  int destiny;
  int npieces;
  int take;
  int space;
  for(int i = 0; i < n; i++) {
    srand(time(NULL));
    while (true) {
      origin = rand() % initial.size;
      npieces = initial.cgs[origin][origin];
      take = (MOVE_PERCENTAGE*npieces) / 100;
      if (take) { // Hay suficientes piezas para mover
        while (true) {
          destiny = rand() % initial.size;
          if (destiny != origin) { 
            space = take*lpiece[origin];
            cout << destiny << endl;
            cout << origin << endl;
            // Se ejecuta movimiento
            if (initial.leftover[destiny] >= space) {
              new_solution->cgs[origin][origin] -= take;
              new_solution->cgs[destiny][origin]  += take;
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
