#ifndef TUNNING
#define TUNNING

//cuttingStock main
#define METAHEURISTIC SCATTER

//SCATTER
#define DIFF_VAR 1
#define DIFF_DISTR 1

//Local Search
#define LSBB_MAXIT 4000 // Cantidad maxima de iteraciones de local search

//Constrain de open stacks y diferencia de tamanos
#define MAX_OS 4        // Maxima cantidad de open stacks abiertos simultaneamente
#define MIN_SD 50       // Minima diferencia de tamano entre los cortes de un mismo cutting group

//Calculo de puntaje de origenes y destinos
#define COMB_OP +       // Operacion para combinar las dos partes de la funcion de puntaje de origenes y destinos
#define OUTSIDE_FUNC    // Funcion que se le aplica al resultado de la funcion de puntaje de origenes y destinos
#define DST_LO_MOD 1    // Modificador del valor del indice de leftover para el destino 
#define DST_FRG_MOD 1   // Modificador del valor del indice de fragmentacion para el destino
#define ORIG_LO_MOD 1   // Modificador del valor del indice de leftover para el origen
#define ORIG_FRG_MOD 1  // Modificador del valor del indice de fragmentacion para el origen

#endif
