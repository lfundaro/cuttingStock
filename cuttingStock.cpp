#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FFD.h"
#include "utilities.h"
#include "localSearchBB.h"
#include "genetic.h"
#include "Solution.h"
using namespace std;

int main(int argc, char *argv[]) {
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
    
    Solution a(rlength, lpiece, dpiece);
    srand(time(NULL));

    vector<Solution*> set = genPeople(ntpieces, rlength, 
                                      lpiece,dpiece);
    
    pair<Solution,Solution> children = Cross(set[0], set[1]);
    cout << "Padre" << endl; 
    set[0]->printSolution();
    cout << "Madre" << endl;
    set[1]->printSolution();
    // cout << "Primer hijo" << endl;
    // children.first.printSolution();
    // cout << "Segundo hijo" << endl;
    // children.second.printSolution();

    Solution k = children.first;
    cout << "ANTES " << endl; 
    k.printSolution();
    fixSolution(k, dpiece, rlength, lpiece);
    cout << "=========== " << endl;
    cout << "DESPUES " << endl; 
    k.printSolution();

    for(int i = 0; i < set.size(); i++) {
      delete set[i];
    }
    
    //    cout << a.cgs[0][0] << endl;
    // vector<vector<int>*> cg = genInitSol(rlength,lpiece,
    //                                      dpiece, leftover,
    //                                      used_rolls,
    //                                      rollType);

    // //FFD(1000,lpiece,*cg[cg.size()-2]);
    // for(i = 0; i < ntpieces; i++) {
    //   if (used_rolls[i]) {
    //     cout << "Tipo " << i << endl;
    //     cout << "rolls " << used_rolls[i] << endl;
    //     cout << "leftover " << leftover[i] << endl;
    //     cout << "n_used_pieces " << cg[i]->at(i) << endl; 
    //     cout << "------------" << endl;
    //   }
    // }
    // cout << "======================\n";
    // localSearchBB(cg, rlength, lot_s, lpiece, dpiece, leftover,
    // 		  used_rolls, variety);
 
    //    free_vector(cg);
    free(line);
    instance.close();
  }
  else 
    cout << "Error leyendo instancia" << endl;

}


    // vector<int> b;
    // b.push_back(74);
    // b.push_back(91);
    // b.push_back(55);
    // b.push_back(100);
    // vector<int> c;
    // c.push_back(15);
    // c.push_back(8);
    // c.push_back(0);
    // c.push_back(10);
    // pair<int,int> a = FFD(1000, b, c);
    // cout << a.first << " " << a.second << endl;



    // vector<int> a;
    // a.push_back(5);
    // a.push_back(0);
    // vector<int> f;
    // f.push_back(2);
    // f.push_back(0);
     
    // pair <int,int> c = FFD(10,a,f);
    
    // cout << "leftover " << c.second << endl;
    // cout << "rolls " << c.first << endl;
   





    // vector<int> f;
    // f.push_back(4);
    // f.push_back(2);
    // vector<int> a;
    // a.push_back(4);
    // a.push_back(2);
    
    // pair<int,vector<vector<int>*> > c = FFD(11,f,a);
    // cout << leftOver(c.second,11, f) << endl;

