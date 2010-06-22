#include "ILS.h"

void ILS(vector<vector<int>*> &cgroups, 
         vector<int> &rlenght, vector<int> &lot_s,
         vector<int> &lpiece, vector<int> &dpiece,
         vector<int> &leftover,vector<int> &used_rolls,
         vector<int> &rollType, vector<int> &variety) {

  int maxit = ILS_MAXIT;

  vector<vector<int>*> S_star;
  vector<vector<int>*> S_pert;
  vector<int> leftover_star;
  vector<int> leftover_pert;
  vector<int> used_rolls_star;
  vector<int> used_rolls_pert;
  vector<int> variety_star;
  vector<int> variety_pert;

  int nitems = lpiece.size();
  int sum;
  int max_pert = MAX_PERTURB;
  double T = INIT_TEMP;


  localSearchBB(cgroups,rlenght,lot_s,
  		lpiece,dpiece,leftover,
  		used_rolls,variety);

  S_star = duplicate(cgroups,nitems);//Para este punto, cgroups a sido procesado
  S_pert = duplicate(S_star,nitems);
  
  leftover_star = leftover;
  used_rolls_star = used_rolls;
  variety_star = variety;

  srand(time(NULL));

  sum = 0;
  int tleft1 = 0;
  int leftoverS1_size = leftover.size();
  for(int i = 0; i < leftoverS1_size; i++) 
    sum += leftover_star[i];
  tleft1 = sum;

  sum = 0;
  int tleft_best;
  int leftover_best_size = leftover.size();
  for(int i = 0; i < leftover_best_size; i++) 
    sum += leftover[i];
  tleft_best = sum;
  
  while (maxit > 0) {
    //cout << maxit <<"\n";
    leftover_pert = leftover_star;
    used_rolls_pert = used_rolls_star;
    variety_pert = variety_star;
    
    //S_pert = duplicate(S_star,nitems);
    overwrite(S_star,S_pert,nitems);
    
    Perturb(S_pert,
    	    leftover_pert,
    	    used_rolls_pert,
    	    variety_pert,
    	    rlenght,
    	    lpiece,
    	    lot_s,
    	    MAX_PERTURB);

    localSearchBB(S_pert, 
    		  rlenght, lot_s,
                  lpiece, dpiece,
    		  leftover_pert,
    		  used_rolls_pert,
    		  variety_pert);

    sum = 0;
    int tleft2;
    int leftoverS2_size = leftover_pert.size();
    for(int i = 0; i < leftoverS2_size; i++) 
      sum += leftover_pert[i];
    tleft2 = sum;

    if (tleft_best > tleft2) {
      cout << maxit <<":"<< tleft2 << "<-Mejora\n";
      leftover = leftover_pert;
      used_rolls = used_rolls_pert;
      variety = variety_pert;
      tleft_best = tleft2;
      overwrite(S_pert,cgroups,nitems);
    }

    if (tleft1 > tleft2) {
      //cout << maxit <<":"<< tleft2 << "<-Mejora*\n";
      leftover_star = leftover_pert;
      used_rolls_star = used_rolls_pert;
      variety_star = variety_pert;
      tleft1 = tleft2;
      overwrite(S_pert,S_star,nitems);
    }
    else if (tleft1 < tleft2){ // Se elige con probabilidad Simulated Annealing
      double p = exp(((double)(tleft1 - tleft2)) / T);
      double r = (double)random()/(double)RAND_MAX;
      if (r < p) {
    	//cout << tleft2 << ":" << tleft1 <<"<-emperorando," << T <<"\n";
    	leftover_star = leftover_pert;
    	used_rolls_star = used_rolls_pert;
    	variety_star = variety_pert;
    	tleft1 = tleft2;
    	overwrite(S_pert,S_star,nitems);
      }
    }
    T *= EXP_DESC; 
    //cout << T << " temp\n";
    maxit--;
  }


  int i;
  int totalLO = 0;
  int totalRollLenght = 0;
  vector<int>* pieceSet; 
  int minLO;
  int minLG;
  int minRollType;

  cout << "Mejor corte\n";
  printCG(cgroups);

  for(i = 0; i < cgroups.size(); i++) {
    sum = 0;
    if (used_rolls[i]) {
      // cout << "Tipo " << i << endl;
      // cout << "rolls " << used_rolls[i] << endl;
      // cout << "leftover " << leftover[i] << endl;
      //cout << leftover[i] << " " << i << " origlo\n";
      totalLO += leftover[i];
      // for(j=0;j<nitems;++j)
      // 	sum += (*cgroups[i])[j];
      //cout << (*cgroups[i])[j] << " map\n";
      // cout << "n_used_pieces " << sum << endl; 
      // cout << "------------" << endl;
    }
  }
  //cout << totalLO <<" original\n";
  totalLO = 0;
  totalRollLenght = 0;
  for(i=0; i<cgroups.size(); ++i){
    if (used_rolls[i]) {
      pieceSet = cgroups[i];
      
      int* bestCut;
      bestCut = bestCutting(*pieceSet, rlenght, lpiece);

      minLO = bestCut[0];
      minLG = bestCut[1];
      minRollType = bestCut[2];

      delete [] bestCut;

      //cout << minLO << " " << i << " minlo\n";
      totalLO += minLO;
      totalRollLenght += minLG*rlenght[minRollType];
    }
  }

  cout << totalLO << "\n";
  cout << totalRollLenght << "\n";
  cout << (100.0*(double)totalLO)/(double)totalRollLenght << "\n";
}

  
  
