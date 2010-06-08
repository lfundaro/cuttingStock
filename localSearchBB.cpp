#include "utilities.h"
#include "localSearchBB.h"
using namespace std;

// Algoritmo de búsqueda local mejor mejor.
void localSearchBB(vector<vector<int>*> &cgroups, 
                   vector<int> &rlenght, vector<int> &lot_s,
                   vector<int> &lpiece, vector<int> &dpiece,
                   vector<int> &leftover,vector<int> &used_rolls) {
  int maxIt = LSBB_MAXIT;
  int info[6]; // arreglo con información sobre el
               // el cambio de vecindad.
  while (maxIt > 0) {
    // info = ShiftN(cgroups, otras cosas...);
    if (group_quality(info, leftover, used_rolls)) {
      // Se ejecuta el movimiento
      // Se actualiza leftover destino
      leftover[info[1]] = info[3]; 
      // Se actualiza used_rolls destino
      used_rolls[info[1]] = info[5];
      // Se actualiza used_rolls origen
      used_rolls[info[4]] = info[4];
      // Se actualiza cgroups destino
      cgroups[info[1]]->at(info[2]) += lot_s[info[2]];
      // Se actualiza cgroups origen
      cgroups[info[0]]->at(info[2]) -= lot_s[info[2]];
    }
    maxIt--;
  }
}
              
