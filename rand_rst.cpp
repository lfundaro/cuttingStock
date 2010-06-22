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
  int old_leftover;// = current_leftover;
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
  leftover.push_back(roll_type);
  variety.push_back(0);
  used_rolls.push_back(1);
  while(pieces_left(dpiece)){
    // //Si no queda espacio, 'introduzco' un nuevo roll
    // if (! space_left(current_leftover, lpiece, lot_s, dpiece, npieces)){
    //   current_leftover = roll_type;
    //   newRolls = 1;
    //   while (! space_left(current_leftover, lpiece, lot_s, dpiece, npieces)){
    // 	current_leftover += roll_type;
    // 	++newRolls;
    //   }
    // }


    //Eligo elementos para introducir en la solucion
    elements = pick(current_leftover, lpiece, lot_s,
		    dpiece, roll_type,npieces);
    
    old_leftover = current_leftover;

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
      used_rolls.push_back(1);
      current_cg++;
      leftover[current_cg] = roll_type;
      // if (newRolls == 0)
      // 	newRolls = 1;
    }

    /*****Actualizo la solucion******/
    leftover[current_cg] -= old_leftover;    /*Quito el leftover anterior
					       que puede estar repetido
					       en element[2]*/
    leftover[current_cg] += elements[2];     //Actualizo leftover
    if ((*sol[current_cg])[elements[0]] == 0)//Actualizo variety
      variety[current_cg] += 1;
    used_rolls[current_cg] += elements[4];           //Actualizo used_rolls
    // //newRolls = 0;
    (*sol[current_cg])[elements[0]] += elements[1];//Actualizo cutting groups

    //Actualizo la demanda
    dpiece[elements[0]] -= elements[1];
    
    //Actualizo el leftover
    current_leftover = elements[3];

    delete [] elements;
  }
  
  return sol;
}

int* pick(int& current_leftover,
	  vector<int>& lpiece,
	  vector<int>& lot_s,
	  vector<int>& dpiece,
	  int roll_size,
	  int npieces){
  int i;
  int k;
  int k1;
  int k2;
  int k3;
  int r;
  int enesimo;
  int demand;
  int* elements;
  int leftover;
  vector<int*> leftovers(0);
  int leftovers_size;

  for(i=0; i<npieces; ++i) {
    demand = dpiece[i];
    if (demand == 0) {continue;}

    k  = current_leftover/lpiece[i];
    k1 = roll_size/lpiece[i];
    k2 = current_leftover/(lpiece[i]*lot_s[i]);
    k3 = (roll_size+k*lpiece[i])/(lpiece[i]*lot_s[i]);
    r  = (lot_s[i]-k)/k1;

    k  = min(k,demand);
    k1 = min(k1,demand);
    k2 = min(k2*lot_s[i],demand);
    k3 = min( k3*lot_s[i]-k , demand-k);

    elements = new int[5];
    elements[0] = i;

    if (k2 > 0){
      elements[1] = k2;
      elements[2] = current_leftover-k2*lpiece[i];
      elements[3] = current_leftover-k2*lpiece[i];
      elements[4] = 0;
    }
    else{
      if (k3 > 0){
	elements[1] = k + k3;
	elements[2] =  current_leftover - k*lpiece[i]\
        	     + roll_size - k3*lpiece[i]+k*lpiece[i];
	elements[3] = roll_size - k3*lpiece[i]-k*lpiece[i];
	elements[4] = 1;
      }
      else{
	elements[1] = min(demand,k + k1*r);
	elements[2] =  current_leftover - k*lpiece[i]\
                     + r*(roll_size-k1*lpiece[i]);
	elements[4] = r;

	if ((lot_s[i]-k) > r*k1){
	  elements[1] += (lot_s[i]-k)-r*k1;
	  elements[2] += roll_size-((lot_s[i]-k)-r*k1)*lpiece[i];
	  elements[3] = roll_size-((lot_s[i]-k)-r*k1)*lpiece[i];
	  elements[4] += 1;
	}
	else
	  elements[3] = roll_size-k1*lpiece[i];
      }
    }

    leftovers.push_back(elements);      
  }

  leftovers_size = leftovers.size();
  sort(leftovers.begin(),leftovers.end(),compareLeftovers);
  enesimo = (random() % min(RCL_SIZE,leftovers_size))+1;

  for (i=0; i<enesimo-1; ++i)
    delete [] leftovers[i];

  for (i=enesimo; i<leftovers_size; ++i)
    delete [] leftovers[i];

  elements = leftovers[enesimo-1];
  current_leftover = elements[3];
  return elements;
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
