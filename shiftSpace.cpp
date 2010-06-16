#include "shiftSpace.h"
using namespace std;

bool compareScores(double* a, double* b){
  return (a[1] > b[1]);
}

bool compareVectors(int a, int b){
  return (a < b);
}

bool checkMove(int orig, int dst, int piece, int npiece,
	       vector<vector<int>*>* cgroups, vector<int>* lpiece,
	       vector<int>* variety){
  int i;
  int npieces = lpiece->size();
  vector<int> sizes_dst(npieces);
  int min = numeric_limits<int>::max();
  
  //Se construye el arreglo con los tamanos
  //de las piezas que existen en el origen
  for(i=0; i<npieces; ++i){
    if ( (*((*cgroups)[dst]))[i] > 0)
      sizes_dst[i] = (*lpiece)[i];
    else
      sizes_dst[i] = 0;
  }

  //Constrain de open stacks
  if ( (*((*cgroups)[orig]))[piece] == 0){
    sizes_dst[piece] = (*lpiece)[piece];
    if ((*variety)[piece]+1 > MAX_OS)
      return false;
  }

  //Constrain de diferencia de tamanos minima
  //Se ordena el arreglo
  sort(sizes_dst.begin(), sizes_dst.end(), compareVectors);
  
  while (sizes_dst[i] == 0){
    ++i;
  }
  
  for (i; i<npieces-1; ++i){
    if ((sizes_dst[i+1]-sizes_dst[i]) < MIN_SD){
      return false;
    }
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
    (*origScores)[i] = new double[2];
    (*dstScores)[i] = new double[2];
  }

  //left overs calc
  for(i=0; i<npieces; ++i)
    if (max < (*leftover)[i])
      max = (*leftover)[i];

  for(i=0; i<npieces; ++i)
    leftO_percentege[i] = (double)(*leftover)[i]/(double)max;

  //frags calc
  //Ciclo sobre los grupos
  for(i=0; i<npieces; ++i){
    //Si el grupo tiene algo dentro
    (*dstScores)[i][0] = i;
    (*origScores)[i][0] = i;
    if ((*used_rolls)[i] != 0){
      num_items = 0;
      sigma = 0;
      //Ciclo sobre los items del grupo
      for(j=0; j<npieces; ++j){
	if ((*cgroups)[i]->at(j) > 0)
	  num_items += (*((*cgroups)[i]))[j];
	sigma += lpiece->at(j);
      }
      double uRolls = (*used_rolls)[i];
      double nItems = num_items;
      (*dstScores)[i][1] = OUTSIDE_FUNC( ((1.0-uRolls/nItems)/*+(1.0-nItems/sigma)*/) COMB_OP (1-leftO_percentege[i]) );
      (*origScores)[i][1] = OUTSIDE_FUNC( ((1.0-uRolls/nItems)/*+(1.0-nItems/sigma)*/) COMB_OP (leftO_percentege[i]) );
    }
    else{
      //El roll estaba vacio
      (*dstScores)[i][1] = -1;
      (*origScores)[i][1] = -1;
    }
  }

  delete [] leftO_percentege;
  sort(dstScores->begin(), dstScores->end(), compareScores);
  sort(origScores->begin(), origScores->end(), compareScores);

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
  int origPos = (*status)[0];
  int dstPos = (*status)[1];

  if (origPos == dstPos)
    if (dstPos+1 < npieces &&
	(*dstScores)[dstPos+1][1] != -1){
      (*status)[0] = origPos;
      (*status)[1] = dstPos + 1;
    }
    else{
      (*status)[0] = origPos + 1;
      (*status)[1] = 0;
    }
  

  int j;
  pair <int,int> temp_result;
  pair <int,int> orig_state;
  int best_stock_size;
  vector<int>* pieceSet;

  int orig;
  int dst;
  int piece;

  int* move = new int[7];

  //Aca va el minimo numero de rolls usados
  move[5] = numeric_limits<int>::max(); 
  move[6] = numeric_limits<int>::max(); 

  //Paro cuando se terminen los origenes
  while (origPos < npieces &&
	 (*origScores)[origPos][1] != -1) {
    piece = 0;
    orig = (*origScores)[origPos][0];
    dst = (*dstScores)[dstPos][0];
    //Mientras halla piezas en el origen acual
    while (piece < npieces){
      //Busco la siguiente pieza que tenga existencias
      while (piece < npieces && (*((*cgroups)[orig]))[piece] == 0)
	piece += 1;

      if (piece >= npieces)
	break;
      
      //Si el movimiento es permitido
      if (checkMove(orig,dst,piece,npieces,
		    cgroups,lpiece,variety)) {
	int nitems_orig = (*((*cgroups)[orig]))[piece];

	//Proceso destino
	pieceSet = (*cgroups)[dst];
	(*pieceSet)[piece] += min((*lot_s)[piece],nitems_orig);
	for(j=0; j<rlenght->size(); ++j){
	  temp_result = FFD((*rlenght)[j],*lpiece,*pieceSet);
	  if (temp_result.first < move[6]){
	    move[6] = temp_result.first; //rolls usados en dst
	    move[4] = temp_result.second;//leftofer en dst
	  }
	}
	(*pieceSet)[piece] -= min((*lot_s)[piece],nitems_orig);

	//Proceso origen
	pieceSet = (*cgroups)[orig];
	(*pieceSet)[piece] -= min((*lot_s)[piece],nitems_orig);
	for(j=0; j<rlenght->size(); ++j){
	  temp_result = FFD((*rlenght)[j],*lpiece,*pieceSet);
	  if (temp_result.first < move[5]){
	    move[5] = temp_result.first; //rolls usados en orig
	    move[3] = temp_result.second;//leftofer en orig
	  }
	}
	(*pieceSet)[piece] += min((*lot_s)[piece],nitems_orig);
	
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
    if (dstPos+1 < npieces &&
	(*dstScores)[dstPos+1][1] != -1)
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
