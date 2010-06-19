#include "rand_rst.h"
using namespace std;

vector<vector<int>*> rand_rst(vector<int>  dpiece,
			      vector<int>& rlength,
			      vector<int>& lpiece,
			      vector<int>& lot_s,
			      vector<int>& leftover,
			      vector<int>& used_rolls,
			      vector<int>& variety) {

  leftover.clear();
  variety.clear();
  used_rolls.clear();

  int npieces = lpiece.size();
  int roll_type = max_item(rlength);

  int current_leftover = roll_type;
  int current_cg = 0;
  int newRolls = 1;

  vector<vector<int>*> sol; //Solucion

  /*Contiene un conjunto de elementos a incluir
    en la solucion.
    elements[0]: tiene el tipo de pieza
    elements[1]: tiene la cantidad de piezas
    elements[2]: tiene el leftover que resultaria
                 de introducir las piezas en el
		 roll
  */
  int* elements;

  sol.push_back(new vector<int>(npieces,0));
  leftover.push_back(0);
  variety.push_back(0);
  used_rolls.push_back(0);
  while(pieces_left(dpiece)){
    //Si no queda espacio, 'introduzco' un nuevo roll
    if (! space_left(current_leftover, lpiece, lot_s, dpiece, npieces)){
      current_leftover = roll_type;
      newRolls = 1;
      while (! space_left(current_leftover, lpiece, lot_s, dpiece, npieces)){
	current_leftover += roll_type;
	++newRolls;
      }
    }


    //Eligo elementos para introducir en la solucion
    elements = pick(current_leftover, lpiece, lot_s, dpiece, npieces);

    //Si no puedo introducir las piezas y mantener
    //el cutting group, abro uno nuevo
    if (! checkMove(current_cg,
		    elements[0],
		    npieces,
		    &sol,
		    &lpiece,
		    &variety)){

      sol.push_back(new vector<int>(npieces,0));
      leftover.push_back(0);
      variety.push_back(0);
      used_rolls.push_back(0);
      current_cg++;
      if (newRolls == 0)
	newRolls = 1;
    }

    /*****Actualizo la solucion******/
    leftover[current_cg] = elements[2];      //Actualizo leftover
    if ((*sol[current_cg])[elements[0]] == 0)//Actualizo variety
      variety[current_cg] += 1;
    used_rolls[current_cg] += newRolls;           //Actualizo used_rolls
    newRolls = 0;
    (*sol[current_cg])[elements[0]] += elements[1];//Actualizo cutting groups

    //Actualizo la demanda
    dpiece[elements[0]] -= elements[1];

    //Actualizo el leftover
    current_leftover = elements[2];
  }
  
  return sol;
}

int* pick(int current_leftover,
	  vector<int>& lpiece,
	  vector<int>& lot_s,
	  vector<int>& dpiece,
	  int npieces){
  int i;
  int k;
  int enesimo;
  int* elements;
  int leftover;
  vector<int*> leftovers(0);

  for(i=0; i<npieces; ++i){
    k = current_leftover/(lpiece[i]*lot_s[i]);
    k = min(k*lot_s[i],dpiece[i]);
    if (k==0) {continue;}

    elements = new int[3];
    elements[0] = i;
    elements[1] = k;
    elements[2] = current_leftover-k*lpiece[i];
    leftovers.push_back(elements);
  }
  
  sort(leftovers.begin(),leftovers.end(),compareLeftovers);
  enesimo = random() % min(RCL_SIZE,leftovers.size()-1);

  leftovers.clear();
  return leftovers[enesimo];
}

bool space_left(int current_leftover,
		vector<int>& lpiece,
		vector<int>& lot_s,
		vector<int>& dpiece,
		int npieces){
  int i;
  int k;

  for(i=0; i<npieces; ++i){
    k = current_leftover/(lpiece[i]*lot_s[i]);
    k = min(k*lot_s[i],dpiece[i]);
    if (k>0) {return true;}
  }
  
  return false;
}

bool pieces_left(vector<int>& dpiece){

  int i;
  int size = dpiece.size();
  int result = 0;

  for(i=0; i<size; i++){
    result += dpiece[i];
  }

  return result;
}

bool compareLeftovers(int* a, int* b){
  return a[2] < b[2];
}
