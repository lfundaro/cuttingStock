#include "shiftSpace.h"
#include "utilities.h"
#include "FFD.h"
#include <math.h>
#define MAX_OS 10000
#define MIN_SD 0
using namespace std;

bool compare(double* a, double* b){
  return (a[1] > b[1]);
}

bool checkMove(int orig, int dst, int piece, int npiece,
	       vector<vector<int>*>* cgroups, vector<int>* lpiece,
	       vector<int>* variety){
  int i;
  int max = numeric_limits<int>::min();
  int min = numeric_limits<int>::max();

  //Constrain de open stacks y de diferencia de tamanos
  if (cgroups->at(orig)->at(piece) == 0){
    //Constrain de diferencia de tamanos
    if (variety->at(piece)+1 > MAX_OS)
      return false;
    
    //Constrain de la diferencia de tamanos minima
    for(i=0; i<npiece; ++i){
      if (max < lpiece->at(cgroups->at(dst)->at(i)))
	max = lpiece->at(cgroups->at(dst)->at(i));

      if (min > lpiece->at(cgroups->at(dst)->at(i)))
	min = lpiece->at(cgroups->at(dst)->at(i));
    }

    if (lpiece->at(piece)<min)
      if (max-lpiece->at(piece) > MIN_SD)
	return false;

    if (lpiece->at(piece)>max)
      if (lpiece->at(piece)-min > MIN_SD)
	return false;
  }
  return true;
}

int initScoreArrays(vector<vector<int>*> *cgroups, vector<int> *used_rolls,
		    vector<int> * lpiece, vector<int>* leftover, int npieces,
		    vector<double*>* origScores, vector<double*>* dstScores){
  int i;
  int j;
  int aux;

  int sigma;
  int num_items;
  int max = 0;

  double* leftO_percentege = new double[npieces];

  //init 2dim arrays in origScores and dstScores
  for(i=0; i<npieces; ++i){
    origScores->at(i) = new double[2];
    dstScores->at(i) = new double[2];
  }

  //left overs calc
  for(i=0; i<npieces; ++i)
    if (max < leftover->at(i))
      max = leftover->at(i);

  for(i=0; i<npieces; ++i)
    leftO_percentege[i] = (double)leftover->at(i)/(double)max;

  //frags calc
  //Ciclo sobre los grupos
  for(i=0; i<npieces; ++i){
    //Si el grupo tiene algo dentro
    dstScores->at(i)[0] = i;
    origScores->at(i)[0] = i;
    if (used_rolls->at(i) != 0){
      num_items = 0;
      sigma = 0;
      //Ciclo sobre los items del grupo
      for(j=0; j<npieces; ++j){
	if (cgroups->at(i)->at(j) > 0)
	  num_items += cgroups->at(i)->at(j);
	sigma += lpiece->at(j);
      }
      double uRolls = used_rolls->at(i);
      double nItems = num_items;
      dstScores->at(i)[1] = sqrt( ((1.0-uRolls/nItems)/*+(1.0-nItems/sigma)*/)+(1-leftO_percentege[i]) );
      origScores->at(i)[1] = sqrt( ((1.0-uRolls/nItems)/*+(1.0-nItems/sigma)*/)+(leftO_percentege[i]) );
    }
    else{
      //El roll estaba vacio
      dstScores->at(i)[1] = -1;
      origScores->at(i)[1] = -1;
    }
  }

  delete [] leftO_percentege;
  sort(dstScores->begin(), dstScores->end(), compare);
  sort(origScores->begin(), origScores->end(), compare);

  // for(i=0;i<npieces;++i){
  //   cout << dstScores->at(i)[0] << ":" <<  (double)dstScores->at(i)[1] << "\n";
  //   cout << origScores->at(i)[0] << ":" << (double)origScores->at(i)[1] << "\n";
  //   cout << "===========\n";
  // }
}

//status: origPos,dstPos
int* next_move(vector<int>* status, int npieces,
	       vector<vector<int>*>* cgroups, vector<int>* variety,
	       vector<int>* lpiece,vector<int>* rlenght, vector<int>* lot_s,
	       vector<double*>* dstScores, vector<double*>* origScores){
  int origPos = status->at(0);
  int dstPos = status->at(1);

  if (origPos == dstPos)
    if (dstPos+1 < npieces &&
	dstScores->at(dstPos+1)[1] != -1){
      status->at(0) = origPos;
      status->at(1) = dstPos + 1;
    }
    else{
      status->at(0) = origPos + 1;
      status->at(1) = 0;
    }
  

  int j;
  pair <int,int> temp_result;
  pair <int,int> orig_state;
  int best_stock_size;
  vector<int>* pieceSet;

  int orig;
  int dst;
  int piece;
  int used_rolls = numeric_limits<int>::max();

  int* move = new int[7];
  //Aca va el minimo numero de rolls usados
  move[4] = numeric_limits<int>::max(); 

  //Paro cuando se terminen los origenes
  while (origPos < npieces &&
	 origScores->at(origPos)[1] != -1) {
    piece = 0;
    orig = origScores->at(origPos)[0];
    dst = dstScores->at(dstPos)[0];
    //Mientras halla piezas en el origen acual
    while (piece < npieces){
      //Busco la siguiente pieza que tenga existencias
      while (piece < npieces && cgroups->at(orig)->at(piece) == 0)
	piece += 1;

      if (piece >= npieces)
	break;
      
      //Si el movimiento es permitido
      if (checkMove(orig,dst,piece,npieces,
		    cgroups,lpiece,variety)) {
	
	pieceSet = cgroups->at(dst);
	pieceSet->at(piece) += lot_s->at(piece);
	for(j=0; j<rlenght->size(); ++j){
	  temp_result = FFD(rlenght->at(j),*lpiece,*pieceSet);
	  if (temp_result.first < move[4]){
	    move[6] = temp_result.first; //rolls usados en dst
	    move[4] = temp_result.second;//leftofer en dst
	  }
	}
	pieceSet->at(piece) -= lot_s->at(piece);

	used_rolls = numeric_limits<int>::max();
	pieceSet = cgroups->at(orig);
	pieceSet->at(piece) -= lot_s->at(piece);
	for(j=0; j<rlenght->size(); ++j){
	  temp_result = FFD(rlenght->at(j),*lpiece,*pieceSet);
	  if (temp_result.first < move[4]){
	    move[5] = temp_result.first; //rolls usados en orig
	    move[3] = temp_result.second;//leftofer en orig
	  }
	}
	pieceSet->at(piece) += lot_s->at(piece);
	
	// //Si le paso la exp directamente FFD se come la ram.
	// //Por alguna razon pasa 0 en vez de el tamano.
	// cgroups->at(orig)->at(piece) -= lot_s->at(piece);
	// int rollsize = rlenght->at(best_stock_size);
	// temp_result = FFD(rollsize,*lpiece,*cgroups->at(orig));
	// move[3] = temp_result.second; //leftover en origen
	// move[5] = temp_result.first;//rolls usados en orig
	// cgroups->at(orig)->at(piece) += lot_s->at(piece);
	    
	//Construyo el resto solucion
	move[0] = orig;
	move[1] = dst;
	move[2] = piece;
	
	//Acualizo status, tiene el siguiente
        //origen y destino a revisar
	if (dstPos+1 < npieces &&
	    dstScores->at(dstPos+1)[1] != -1){
	  status->at(0) = origPos;
	  status->at(1) = dstPos + 1;
	}
	else{
	  status->at(0) = origPos + 1;
	  status->at(1) = 0;
	}
	
	return move;
      }
      else
	//Siguiente pieza
	piece += 1;
    }

    //Si llego aca es que ya probe todas las
    //piezas para este origen y destino, calculo
    //un siguiente destino y si era el ultimo
    //un siguiente origen
    if (dstPos+1 < npieces &&
	dstScores->at(dstPos+1)[1] != -1)
      dstPos += 1;
    else{
      origPos += 1;
      dstPos = 0;
    }
  } 
  //cout << origScores->at(origPos)[1] <<"\n";

  move[0] = -1;
  move[1] = -1;
  move[2] = -1;

  return move;
}
