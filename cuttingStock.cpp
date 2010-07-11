#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FFD.h"
#include "utilities.h"
#include "localSearchBB.h"
#include "genetic.h"
#include "scatterSearch.h"
#include "Solution.h"

#include <sys/time.h>
#include <sys/resource.h>
using namespace std;

int main(int argc, char *argv[]) {
  cout << RLIMIT_STACK << "\n";
  
  char * line = (char *) malloc(sizeof(char)*60);
  ifstream instance(argv[1]);
  int i;
  if (instance.is_open()) {
    int nroll_type; // Número de Stock Roll type
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &nroll_type);
    int ntpieces;  // Número de tipo piezas 
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &ntpieces);
    // Se leen nroll_type líneas para almacenar 
    // las longitudes de cada tipo de roll.
    vector<int> rlength(nroll_type);
    for(i = 0; i < nroll_type; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d", &rlength[i]);
    }
    vector<int> lot_s(ntpieces); // Tamaño de lote
    vector<int> lpiece(ntpieces); // Longitud de pieza
    vector<int> dpiece(ntpieces); // Demanda de pieza
    // Se leen ntpieces líneas para almacenar 
    // la longitud de cada pieza, demanda y tamaño de 
    // lote
    for(i = 0; i < ntpieces; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d %d %d",&lpiece[i],&dpiece[i],&lot_s[i]);
    }

    // Arreglo de desperdicios por cutting group
    vector<int> leftover(ntpieces);
    for(i = 0; i < ntpieces; i++) 
      leftover[i] = 0;
    
    // Vector de Rolls usados por cada cutting group
    vector<int> used_rolls(ntpieces);
    for(i = 0; i < ntpieces; i++) 
      used_rolls[i] = 0;
    
    // Vector que indica el tipo de roll que se usa 
    // en la i-ésima posición de la solución
    vector<int> rollType(ntpieces);
    for(i = 0; i < ntpieces; i++) 
      rollType[i] = -1;

    vector<int> variety(ntpieces,1);
    
    srand(time(NULL));

    vector<Solution> a = genPset(rlength,lpiece,dpiece,100,lot_s);
    cout << "==============\n";
    vector<Solution> b = makeRefSet(a,10);

    for (int i = 0; i<10; ++i){
      printCG(b[i].cgs);
      cout << "==============\n";
    }

    // Solution initial = Solution(rlength,lpiece,dpiece);
    // Solution a = randomSol(initial,lpiece,rlength);
    // a.printSolution();
    // cout << "\n";
    // Solution initial = Solution(rlength,lpiece,dpiece);
    // Solution a = randomSol(initial,lpiece,rlength);
    // Solution b = randomSol(initial,lpiece,rlength);
    // printCG(initial.cgs);
    // cout << "==============\n";
    // printCG(b.cgs);
    // cout << (double)diff(initial,b);

    // localSearchBB(a,rlength,lot_s,lpiece,dpiece);

    // int ntpiecesD = (double) ntpieces;
    // int tamPoblacion = round(ntpieces + FRACC*ntpieces);
    // Solution a = geneticAlgorithm(tamPoblacion,
    //                               rlength,dpiece,
    //                               lpiece, MAX_IT);
    vector<Solution> a = genPset(rlength,lpiece,dpiece,10,lot_s);
    // int* pair;
    // int candidates[10];
    // for(int i = 0; i < 10; i++) candidates[i] = i;
    // int next_swap[2] = {0,2};
    // pair = twoOnN(candidates,next_swap,10);
    // while(pair != NULL) {
    //   cout << pair[0] << " " << pair[1] << endl;
    //   free(pair);
    //   pair = twoOnN(candidates,next_swap,10);
    // }
    //    a.printSolution();
    //    a.printAsPaper(rlength,lpiece);
    // for(int i = 0; i < 10; i++) 
    //   a[i].printSolution();
    //    Solution a = Solution(rlength,lpiece,dpiece);
    // Solution ram = randomSol(a,lpiece,rlength);
    // a.printSolution();
    // a.printAsPaper(rlength,lpiece);
    free(line);
    instance.close();
  }
  else 
    cout << "Error leyendo instancia" << endl;

}

    // Solution a = Solution(rlength,lpiece,dpiece);
    // vector<Solution> pool;
    // Solution ramdSol;
    // vector<pair<int,double> > control;   
    // Solution initial = Solution(rlength, lpiece, dpiece);
    // for(int i = 0; i < 10; i++) {
    //   ramdSol = randomSol(initial, lpiece, rlength);
    //   ramdSol.fitnessEval();
      
    //   pool.push_back(ramdSol);
    // }


    // Solution ram = randomSol(a,lpiece,rlength);
    // ram.fitnessEval();
    // Solution ram2 = randomSol(a,lpiece,rlength);
    // ram2.fitnessEval();
    // Solution ram3 = randomSol(a,lpiece,rlength);
    // ram3.fitnessEval();
    // ram.printSolution();
    // cout << "===========" << endl;
    // ram2.printSolution();
    // cout << "===========" << endl;
    // ram3.printSolution();
