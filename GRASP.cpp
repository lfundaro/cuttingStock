#include "GRASP.h"

void GRASP(vector<vector<int>*> &cgroups, 
         vector<int> &rlenght, vector<int> &lot_s,
         vector<int> &lpiece, vector<int> &dpiece,
         vector<int> &leftover,vector<int> &used_rolls,
         vector<int> &rollType, vector<int> &variety) {

  int maxit = GRASP_MAXIT;

  vector<vector<int>*> S_rand;
  vector<int> leftover_rand(0);
  vector<int> used_rolls_rand(0);
  vector<int> variety_rand(0);

  int nitems = lpiece.size();
  int sum;

  srand(time(NULL));
  while (maxit > 0) {
    //cout << maxit <<"\n";
    
    S_rand = rand_rst(dpiece,
		      rlenght,
		      lpiece,
		      lot_s,
		      leftover_rand,
		      used_rolls_rand,
		      variety_rand);

    /*Los leftovers que da rand_rst para la configuracion
    que calcula es una cota superior para los mejores leftovers
    posibles de esa misma configuracion. Aca se calculan
    los mejores posibles antes de llamar a localSearch
    */
    for(int i=0; i<S_rand.size(); ++i){
      if (used_rolls_rand[i]) {
	vector<int>* pieceSet;
	int* bestCut;

	pieceSet = S_rand[i];
	
	bestCut = bestCutting(*pieceSet, rlenght, lpiece);
	
	leftover_rand[i] = bestCut[0];
	used_rolls_rand[i] = bestCut[1];
	
	delete [] bestCut;
	
      }
      else
	leftover_rand[i] = 0;
    }

    localSearchBB(S_rand, 
    		  rlenght,
    		  lot_s,
                  lpiece,
    		  dpiece,
    		  leftover_rand,
    		  used_rolls_rand,
    		  variety_rand);
    
    sum = 0;
    int tleft_rand;
    int leftover_rand_size = leftover_rand.size();
    for(int i = 0; i < leftover_rand_size; i++) 
      if (used_rolls_rand[i])
	sum += leftover_rand[i];
    tleft_rand = sum;

    sum = 0;
    int tleft_best;
    int leftover_best_size = leftover.size();
    for(int i = 0; i < leftover_best_size; i++) 
      if (used_rolls[i])
	sum += leftover[i];
    tleft_best = sum;

    if (tleft_best > tleft_rand) {
      cout << maxit <<":"<< tleft_rand << "<-Mejora\n";
      leftover = leftover_rand;
      used_rolls = used_rolls_rand;
      variety = variety_rand;
      substitute(S_rand,cgroups,nitems);
    }
    maxit--;
  }

  int i;
  int j;
  int totalLO = 0;
  int totalRollLenght = 0;
  vector<int>* pieceSet; 
  int minLO;
  int minLG;
  int minRollType;

  cout << "Mejor grupo\n";
  printCG(cgroups);

  for(i = 0; i < cgroups.size(); i++) {
    sum = 0;
    if (used_rolls[i]) {
      // cout << "Tipo " << i << endl;
      // cout << "rolls " << used_rolls[i] << endl;
      // cout << "leftover " << leftover[i] << endl;
      // cout << leftover[i] << " " << i << " origlo\n";
      totalLO += leftover[i];
      // for(j=0;j<nitems;++j)
      // 	sum += (*cgroups[i])[j];
      // cout << "n_used_pieces " << sum << endl; 
      // cout << "------------" << endl;
    }
  }

  totalRollLenght = 0;
  for(i=0; i<cgroups.size(); ++i){
    if (used_rolls[i]) {
      pieceSet = cgroups[i];
      
      int* bestCut;
      bestCut = bestCutting(*pieceSet, rlenght, lpiece);

      minLG = bestCut[1];
      minRollType = bestCut[2];

      delete [] bestCut;

      totalRollLenght += minLG*rlenght[minRollType];
    }
  }

  cout << totalLO << "\n";
  cout << totalRollLenght << "\n";
  cout << (100.0*(double)totalLO)/(double)totalRollLenght << "\n";
}

  
  
