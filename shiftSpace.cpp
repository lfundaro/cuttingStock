#include "shiftSpace.h"
#include "utilities.h"
#include "FFD.h"
#define MAX_OS 4
#define MIN_SD 100 
using namespace std;

bool compare(int* a, int* b){
  return (a[1] > b[1]);
}

bool checkMove(int orig, int dst, int piece, int npiece,
	       vector<vector<int>*>* cgroups, vector<int>* lpiece,
	       vector<int>* bpiece, vector<int>* variety){
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
}

int initScoreArrays(vector<vector<int>*>* cgroups, vector<int>* used_rolls,
		    vector<int>* lpiece, vector<int>* leftover, int npieces,
		    vector<int*>* origScores, vector<int*>* dstScores){
  int i;
  int j;
  int aux;

  int sigma;
  int num_items;
  int max = 0;

  int* leftO_percentege = new int[npieces];
  
  //left overs calc
  for(i=0; i<npieces; ++i)
    if (max < leftover->at(i))
      max = leftover->at(i);

  for(i=0; i<npieces; ++i)
    leftO_percentege[i] = leftover->at(i)*100/max;

  //frags calc
  //Ciclo sobre los grupos
  for(i=0; i<npieces; ++i){
    //Si el grupo tiene algo dentro
    if (used_rolls->at(i) != 0){
      num_items = 0;
      sigma = 0;
      //Ciclo sobre los items del grupo
      for(j=0; j<npieces; ++j){
	if (cgroups->at(i)->at(j) > 0)
	  num_items += cgroups->at(i)->at(j);
	sigma += lpiece->at(j);
      }
      dstScores->at(i)[1] = ((num_items/used_rolls->at(i))+(sigma/num_items))*(1-leftO_percentege[i]);
      origScores->at(i)[1] = ((num_items/used_rolls->at(i))+(sigma/num_items))*(leftO_percentege[i]);
    }
    else{
      //El roll estaba vacio
      dstScores->at(i)[1] = -1;
      origScores->at(i)[1] = -1;
    }
  }

  sort(dstScores->begin(), dstScores->end(), compare);
  sort(origScores->begin(), origScores->end(), compare);
}

//status: origPos,dstPos
int* next_move(vector<int>* status, int npieces,
	       vector<vector<int>*>* cgroups, vector<int>* variety,
	       vector<int>* lpiece,vector<int>* bpiece,vector<int>* rlenght,
	       vector<int*>* dstScores, vector<int*>* origScores){
  int j;
  int temp;
  int best_stock_size;
  vector<int>* pieceSet;

  int origPos = status->at(0);
  int dstPos = status->at(1);

  int orig;
  int dst;
  int piece;
  int used_rolls = numeric_limits<int>::max();

  int* move = new int[6];
  //Aca va el minimo numero de rolls usados
  move[4] = numeric_limits<int>::max(); 

  //Paro cuando se terminen los origenes
  while (origScores->at(origPos)[1] != -1){
    piece=0;
    orig = origScores->at(origPos)[0];
    dst = dstScores->at(dstPos)[0];
    //Mientras halla piezas en el origen acual
    while (piece < npieces){
      //Busco la siguiente pieza que tenga existencias
      while (cgroups->at(orig)->at(piece) == 0)
	piece += 1;
      
      //Si el movimiento es permitido
      if (checkMove(orig,dst,piece,npieces,
		    cgroups,lpiece,bpiece,
		    variety)){
	
	pieceSet = cgroups->at(dst);
	for(j=0; j<rlenght->size(); ++j){
	  temp = FFD(rlenght->at(j),*lpiece,*pieceSet).first;
	  if (temp<move[4]){
	    move[4] = temp;
	    best_stock_size = j;
	  }
	}
	
	move[5] = FFD(best_stock_size,*lpiece,*cgroups->at(orig)).first;
	    
	//Construyo solucion
	move[0] = orig;
	move[1] = dst;
	move[2] = piece;
	move[3] = 0; //leftover(cgroups->at(dst))
	
	//Acualizo status, tiene el siguiente
        //origen y destino a revisar
	if (dstScores->at(dstPos+1)[1] != -1)
	  status->at(1) = dstPos + 1;
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
    if (dstScores->at(origPos+1)[1] != -1)
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
