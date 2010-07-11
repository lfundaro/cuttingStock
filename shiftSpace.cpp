#include "shiftSpace.h"
using namespace std;

bool compareScores(double* a, double* b){
  return (a[1] > b[1]);
}

bool compareScoresInverse(double* a, double* b){
  if ((a[1] == -1) && (b[1] != -1))
    return false;
  else if((b[1] == -1) && (a[1] != -1))
    return true;
  else
    return (a[1] < b[1]);
}

bool compareVectors(int a, int b){
  return (a < b);
}

bool checkMove(int dst, int piece, int npiece,
	       vector< vector<int> >* cgroups, vector<int>* lpiece,
	       vector<int>* variety){
  int i;
  int npieces = lpiece->size();
  vector<int> sizes_dst(npieces);
  int min = numeric_limits<int>::max();
  
  //Se construye el arreglo con los tamanos
  //de las piezas que existen en el destino
  for(i=0; i<npieces; ++i){
    if ( ((*cgroups)[dst])[i] > 0)
      sizes_dst[i] = (*lpiece)[i];
    else
      sizes_dst[i] = 0;
  }

  //Constrain de open stacks
  if ( ((*cgroups)[dst])[piece] == 0 ){
    sizes_dst[piece] = (*lpiece)[piece];
    if ((*variety)[dst]+1 > MAX_OS)
      return false;
  }

  //Constrain de diferencia de tamanos minima
  //Se ordena el arreglo
  sort(sizes_dst.begin(), sizes_dst.end(), compareVectors);
  
  i=0;
  while ((i<npieces)&&(sizes_dst[i] == 0)){
    ++i;
  }
  
  for (i; i<npieces-1; ++i){
    if ((sizes_dst[i+1]-sizes_dst[i]) < MIN_SD){
      return false;
    }
  }
  
  return true;
}

int initScoreArrays(vector< vector<int> > *cgroups, vector<int> *used_rolls,
		    vector<int> * lpiece, vector<int>* leftover, 
		    vector<int>* rollType, int npieces, 
		    vector<double*>* origScores, vector<double*>* dstScores){
  int i;
  int j;
  int aux;

  int sigma;
  int num_items;
  int max = 0;
  int ngroups = cgroups->size();

  double* leftO_percentege = new double[ngroups];

  //left overs calc
  for(i=0; i<ngroups; ++i)
    if (max < (*leftover)[i])
      max = (*leftover)[i];

  for(i=0; i<ngroups; ++i)
    leftO_percentege[i] = (double)(*leftover)[i]/(double)max;

  //frags calc
  //Ciclo sobre los grupos
  for(i=0; i<ngroups; ++i){
    (*dstScores)[i][0] = i;
    (*origScores)[i][0] = i;
    //Si el grupo tiene algo dentro
    if ((*used_rolls)[i] != 0){
      num_items = 0;
      sigma = 0;
      //Ciclo sobre los items del grupo
      for(j=0; j<npieces; ++j){
	if ( ((*cgroups)[i])[j] > 0 )
	  num_items += ((*cgroups)[i])[j];
	sigma += lpiece->at(j);
      }
      double uRolls = (*used_rolls)[i];
      double nItems = num_items;
      (*dstScores)[i][1] = OUTSIDE_FUNC((  DST_FRG_MOD*(1.0-uRolls/nItems)/*+(1.0-nItems/sigma)*/) COMB_OP (DST_LO_MOD *(1-leftO_percentege[i])));
      (*origScores)[i][1] = OUTSIDE_FUNC((ORIG_FRG_MOD*(1.0-uRolls/nItems)/*+(1.0-nItems/sigma)*/) COMB_OP (ORIG_LO_MOD*(leftO_percentege[i])));
    }
    else{
      //El roll estaba vacio
      (*dstScores)[i][1] = -1;
      (*origScores)[i][1] = -1;
    }
  }

  delete [] leftO_percentege;
  sort(dstScores->begin(), dstScores->end(), compareScoresInverse);
  sort(origScores->begin(), origScores->end(), compareScores);
  
  // for (i=0; i<dstScores->size(); ++i){
  //   cout << dstScores->at(i)[1] <<" dst\n";
  // }
  // cout << "--------------\n";
  // for (i=0; i<dstScores->size(); ++i){
  //   cout << origScores->at(i)[1] <<" orig\n";
  // }
  // cout << "===============\n";
}

//status: origPos,dstPos
int* next_move(vector<int>* status, int npieces,
	       vector< vector<int> >* cgroups, vector<int>* variety,
	       vector<int>* rollType, vector<int>* lpiece,
	       vector<int>* rlenght, vector<int>* lot_s,
	       vector<double*>* dstScores, vector<double*>* origScores){
  int ngroups = cgroups->size();
  int origPos = (*status)[0];
  int dstPos = (*status)[1];

  if (origPos == dstPos)
    if (dstPos+1 < ngroups &&
	(*dstScores)[dstPos+1][1] != -1){
      (*status)[0] = origPos;
      (*status)[1] = dstPos + 1;
    }
    else{
      (*status)[0] = origPos + 1;
      (*status)[1] = 0;
    }
  

  int i;
  int j;
  bool safe_move;
  pair <int,int> temp_result;
  pair <int,int> orig_state;
  int best_stock_size;
  vector<int>* pieceSet;

  int orig;
  int dst;
  int piece;

  int* move = new int[9];

  //Paro cuando se terminen los origenes
  while (origPos < ngroups &&
	 (*origScores)[origPos][1] != -1) {
    piece = 0;
    orig = (*origScores)[origPos][0];
    dst = (*dstScores)[dstPos][0];

    if (orig == dst){
      if (dstPos+1 < ngroups &&
	  (*dstScores)[dstPos+1][1] != -1)
	dstPos += 1;
      else{
	origPos += 1;
	dstPos = 0;
      }
      continue;
    }

    //Mientras halla piezas en el origen acual
    while (piece < npieces){
      //Busco la siguiente pieza que tenga existencias
      while (piece < npieces && ((*cgroups)[orig])[piece] == 0)
	piece += 1;

      if (piece >= npieces)
	break;
      
      //Si el movimiento es permitido
      if (checkMove(dst,piece,npieces,
		    cgroups,lpiece,variety)) {
	int nitems_orig = ((*cgroups)[orig])[piece];
	int items_to_move = min((*lot_s)[piece],nitems_orig);;
	int* bestCut;

	//Proceso destino
	pieceSet = &((*cgroups)[dst]);
	(*pieceSet)[piece] += items_to_move;

	bestCut = bestCutting(*pieceSet,*rlenght,*lpiece);
	move[4] = bestCut[0]; //leftover
	move[6] = bestCut[1]; //rollos usados
	move[8] = bestCut[2]; //tipo de rollo
	delete [] bestCut;

	(*pieceSet)[piece] -= items_to_move;

	//Proceso origen
	pieceSet = &((*cgroups)[orig]);
	(*pieceSet)[piece] -= items_to_move;

	bestCut = bestCutting(*pieceSet,*rlenght,*lpiece);
	move[3] = bestCut[0]; //leftover
	move[5] = bestCut[1]; //rollos usados
	move[7] = bestCut[2]; //tipo de rollo
	delete [] bestCut;

	(*pieceSet)[piece] += items_to_move;

	//Termino de construir la solucion
	move[0] = orig;
	move[1] = dst;
	move[2] = piece;
	
	//Acualizo status, tiene el siguiente
        //origen y destino a revisar
	if (dstPos+1 < ngroups &&
	    ((*dstScores)[dstPos+1])[1] != -1){
	  (*status)[0] = origPos;
	  (*status)[1] = dstPos + 1;
	}
	else{
	  (*status)[0] = origPos + 1;
	  (*status)[1] = 0;
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
    if (dstPos+1 < ngroups &&
	(*dstScores)[dstPos+1][1] != -1)
      dstPos += 1;
    else{
      origPos += 1;
      dstPos = 0;
    }
  } 

  move[0] = -1;
  move[1] = -1;
  move[2] = -1;

  return move;
}
