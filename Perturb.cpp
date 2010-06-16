#include "Perturb.h"
using namespace std;

//vector<vector<int>*>
void
Perturb(vector<vector<int>*> &S_star,vector<int> &leftover,
       vector<int> &used_rolls,vector<int> &variety,
       vector<int> &rlenght,vector<int> &lpiece,vector<int> &lot_s,
       int pertubs) {

  int i;
  int j;
  int orig = 0;
  int dst = 0;
  int piece = 0;
  int npieces = lpiece.size();
  int nrolls = rlenght.size();
  int uRolls = numeric_limits<int>::max();
  int bestLeftO;
  int bestRolls;

  vector<vector<int>*> S_pert = duplicate(S_star, npieces);
  vector<int> leftover_pert = leftover;
  vector<int>* pieceSet;
  pair <int,int> temp_result;
  pair<vector<int>,vector<vector<int>*> > result;


  while( i < pertubs ){
    orig = (int)round(random()) % npieces;
    dst = (int)round(random()) % npieces;
    piece = (int)round(random()) % npieces;    

    //Si el movimiento es permitido
    if (checkMove(orig,dst,piece,npieces,
		  &S_pert,&lpiece,&variety)) {
      int nitems_orig = (*S_pert[orig])[piece]; 

      //======Destino
      if ((*S_pert[dst])[piece] == 0)
        variety[dst]++;
    
      pieceSet = S_pert[dst];
      (*pieceSet)[piece] += min(lot_s[piece],nitems_orig);
      for(j=0; j<nrolls; ++j){
	temp_result = FFD(rlenght[j],lpiece,*pieceSet);
	if (temp_result.first < uRolls){
	  bestRolls = temp_result.first; //rolls usados en dst
	  bestLeftO = temp_result.second;//leftofer en dst
	}
      }
      leftover[dst] = bestLeftO;
      used_rolls[dst] = bestRolls;
    

      //======Origen
      uRolls = numeric_limits<int>::max();
      pieceSet = S_pert[orig];
      (*pieceSet)[piece] -=  min(lot_s[piece],nitems_orig);
      for(j=0; j<nrolls; ++j){
	temp_result = FFD(rlenght[j],lpiece,*pieceSet);
	if (temp_result.first < uRolls){
	  bestRolls = temp_result.first; //rolls usados en orig
	  bestLeftO = temp_result.second;//leftofer en orig
	}
      }
      leftover[orig] = bestLeftO;
      used_rolls[orig] = bestRolls;

      if (S_pert[orig]->at(piece) == 0)
        variety[dst]--;

      i++;
    }
  }
  //return S_pert;
}

