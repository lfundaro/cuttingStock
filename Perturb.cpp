#include "Perturb.h"
using namespace std;

void
Perturb(vector<vector<int>*> &S_pert,vector<int> &leftover,
       vector<int> &used_rolls,vector<int> &variety,
       vector<int> &rlenght,vector<int> &lpiece,vector<int> &lot_s,
       int pertubs) {

  int i = 0;
  int orig;
  int dst;
  int piece;
  int npieces = lpiece.size();

  vector<int>* pieceSet;

  while( i < pertubs ){
    orig = (int)round(random()) % npieces;
    dst = (int)round(random()) % npieces;
    piece = (int)round(random()) % npieces;    
    
    int nitems_orig = (*S_pert[orig])[piece]; 

    if (orig == dst){
      continue;
    }

    if (nitems_orig == 0){
      continue;
    }

    //Si el movimiento es permitido
    if (checkMove(dst,piece,npieces,
		  &S_pert,&lpiece,&variety)) {
      int* bestCut;

      //======Destino
      if ((*S_pert[dst])[piece] == 0)
        variety[dst]++;

      pieceSet = S_pert[dst];
      (*pieceSet)[piece] += min(lot_s[piece],nitems_orig);
      
      bestCut = bestCutting(*pieceSet,rlenght,lpiece);

      leftover[dst] = bestCut[0];//leftofer en dst
      used_rolls[dst] = bestCut[1]; //rolls usados en dst

      delete [] bestCut;

      //======Origen
      pieceSet = S_pert[orig];
      (*pieceSet)[piece] -=  min(lot_s[piece],nitems_orig);

      bestCut = bestCutting(*pieceSet,rlenght,lpiece);

      leftover[orig] = bestCut[0];//leftofer en dst
      used_rolls[orig] = bestCut[1]; //rolls usados en dst

      if ((*S_pert[orig])[piece] == 0)
        variety[orig]--;

      delete [] bestCut;

      i++;
    }
  }
}

