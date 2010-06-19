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
  int k;
  int totalLO = 0;
  int totalRollLenght = 0;
  bool safe_move;
  vector<int>* pieceSet; 
  int minLO;
  int minLG;
  int minRollType;
  pair <int,int> ffdresult;

  for(i = 0; i < cgroups.size(); i++) {
    sum = 0;
    if (used_rolls[i]) {
      // cout << "Tipo " << i << endl;
      // cout << "rolls " << used_rolls[i] << endl;
      // cout << "leftover " << leftover[i] << endl;
      cout << leftover[i] << " " << i << " origlo\n";
      totalLO += leftover[i];
      // for(j=0;j<nitems;++j)
      // 	sum += (*cgroups[i])[j];
      // cout << "n_used_pieces " << sum << endl; 
      // cout << "------------" << endl;
    }
  }
  cout << totalLO <<" original\n";
  totalLO = 0;
  totalRollLenght = 0;
  for(i=0; i<cgroups.size(); ++i){
    if (used_rolls[i]) {
      minLO = MAX_INT;
      minLG = MAX_INT;
      pieceSet = cgroups[i];
      
      for(j = 0; j < rlenght.size(); j++) {
	safe_move = true;
	//Recorro las piezas del pieceSet
	for(k=0; k<nitems; k++){
	  if ((*pieceSet)[k] > 0)//Si hay piezas
	    if (lpiece[k] > rlenght[j]){//Si el largo de esa pieza es muy grande
	      safe_move = false;
	      break;
	    }
	}
	
	if (safe_move){
	  ffdresult = FFD(rlenght[j], lpiece, *pieceSet);
	  if (ffdresult.first < minLG){
	    minLO = ffdresult.second;
	    minLG = ffdresult.first;
	    minRollType = j;
	  }
	}
      }
      cout << minLO << " " << i << " minlo\n";
      totalLO += minLO;
      totalRollLenght += minLG*rlenght[minRollType];
    }
  }
  cout << totalLO << "\n";
  cout << totalRollLenght << "\n";
  cout << (100.0*(double)totalLO)/(double)totalRollLenght << "\n";

}

  
  
