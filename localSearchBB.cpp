#include "utilities.h"
#include "localSearchBB.h"
using namespace std;

// Algoritmo de búsqueda local mejor mejor.
void localSearchBB(vector<vector<int>*> &cgroups, 
                   vector<int> &rlenght, vector<int> &lot_s,
                   vector<int> &lpiece, vector<int> &dpiece,
                   vector<int> &leftover,vector<int> &used_rolls,
		   vector<int> &variety) {
  vector<double*> origScores(cgroups.size());
  vector<double*> dstScores(cgroups.size());
  vector<int> status(2,0);
  int maxIt = LSBB_MAXIT;
  int *info; // arreglo con información sobre el
               // el cambio de vecindad.
  //init 2dim arrays in origScores and dstScores
  for(int i=0; i<cgroups.size(); ++i){
    origScores[i] = new double[2];
    dstScores[i] = new double[2];
  }

  initScoreArrays(&cgroups, &used_rolls, &lpiece, &leftover, lpiece.size(),
		  &origScores, &dstScores);
  while (maxIt > 0 && status[0] < lpiece.size()) {
    info = next_move(&status, lpiece.size(), &cgroups, &variety,
     		     &lpiece, &rlenght, &lot_s ,&dstScores, &origScores);
    if (group_quality(info, leftover, used_rolls)) {
      // Se ejecuta el movimiento
      // Se actualiza leftover origen
      leftover[info[0]] = info[3]; 
      // Se actualiza leftover destino
      leftover[info[1]] = info[4]; 

      // Se actualiza used_rolls origen
      used_rolls[info[0]] = info[5];
      // Se actualiza used_rolls destino
      used_rolls[info[1]] = info[6];

      // Se actualiza estructura variety para destino
      if (cgroups[info[1]]->at(info[2]) == 0)
	variety[info[1]]++;

      int nitems_orig = cgroups[info[0]]->at(info[2]);
      // Se actualiza cgroups destino
      cgroups[info[1]]->at(info[2]) += min(nitems_orig,lot_s[info[2]]);
      // Se actualiza cgroups origen
      cgroups[info[0]]->at(info[2]) -= min(nitems_orig,lot_s[info[2]]);

      // Se actualiza estructura variety para origen
      if (cgroups[info[0]]->at(info[2]) == 0) 
	variety[info[0]]--; 

      //origScores.clear();
      //origScores.resize(cgroups.size());
      //dstScores.clear();
      //dstScores.resize(cgroups.size());
      status[0]=0;
      status[1]=0;
      initScoreArrays(&cgroups, &used_rolls, &lpiece, &leftover, lpiece.size(),
		      &origScores, &dstScores);
    }
    maxIt--;
    delete [] info;
  }
  origScores.clear();
  dstScores.clear();
  // int i;
  // int j;
  // int sum;
  // for(i = 0; i < lpiece.size(); i++) {
  //   sum = 0;
  //   if (used_rolls[i]) {
  //     cout << "Tipo " << i << endl;
  //     cout << "rolls " << used_rolls[i] << endl;
  //     cout << "leftover " << leftover[i] << endl;
  //     for(j=0;j<lpiece.size();++j)
  // 	sum += cgroups[i]->at(j);
  // 	cout << (*cgroups[i])[j] << " map\n";
  //     cout << "n_used_pieces " << sum << endl; 
  //     cout << "------------" << endl;
  //     }
  // }
}
              
