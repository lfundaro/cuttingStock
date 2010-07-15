#include "global_vars.h"

/*Scatter*/
int Psize;
int refSize;
int recons; //Numero de reconstrucciones del refSet
float min_diff_solutions; //Ver Nota al final del archivo

/*Constrains*/
int max_open_stacks;// Maxima cantidad de open stacks abiertos simultaneamente
int min_size_diff;  // Minima diferencia de tamano entre los cortes de un mismo cutting group

/*Local Search*/
int localSearchMaxIts;// Cantidad maxima de iteraciones de local search

/*Move evaluation*/
int dst_lo_mod;   // Modificador del valor del indice de leftover para el destino 
int dst_frg_mod;  // Modificador del valor del indice de fragmentacion para el destino
int orig_lo_mod;  // Modificador del valor del indice de leftover para el origen
int orig_frg_mod; // Modificador del valor del indice de fragmentacion para el origen

//NOTA: min_diff_solutions es:
/*La diferencia minima entre dos soluciones
  para que el algoritmo de generacion del
  conjunto P la acepte
*/
