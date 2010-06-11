#include "utilities.h"
#include "localSearchBB.h"
#include "shiftSpace.cpp"
using namespace std;

// Algoritmo de búsqueda local mejor mejor.
void localSearchBB(vector<vector<int>*> &cgroups, 
                   vector<int> &rlenght, vector<int> &lot_s,
                   vector<int> &lpiece, vector<int> &dpiece,
                   vector<int> &leftover,vector<int> &used_rolls,
		   vector<int> &variety) {
  vector<int*> origScores(lpiece.size());
  vector<int*> dstScores(lpiece.size());
  vector<int> status(2,0);
  int maxIt = LSBB_MAXIT;
  int *info; // arreglo con información sobre el
               // el cambio de vecindad.
  initScoreArrays(&cgroups, &used_rolls, &lpiece, &leftover, lpiece.size(),
		  &origScores, &dstScores);
  while (maxIt > 0) {
    info = next_move(&status, lpiece.size(), &cgroups, &variety,
     		     &lpiece, &rlenght, &dstScores, &origScores);
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
      // Se actualiza estructura variety para destino
      if (cgroups[info[1]]->at(info[2]) == 0)
	variety[info[1]]++;
      // Se actualiza estructura variety para origen
      if (cgroups[info[0]]->at(info[2]) == 0) 
	variety[info[0]]--; 
    }
    maxIt--;
    delete [] info;
  }
}
              
