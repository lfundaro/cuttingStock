#ifndef TUNNING
#define TUNNING

//cuttingStock main
#define METAHEURISTIC ILS

//ILS
#define ILS_MAXIT 2000  // Cantidad de interaciones de ILS
#define MAX_PERTURB 4   // Fuerza de la perturbacion
#define INIT_TEMP 1000  // Temperatura inicial (ver NOTA)
#define EXP_DESC 0.999  // Descenso exponencial de la temperatura (ver NOTA)

//GRASP
#define GRASP_MAXIT 1000  // Cantidad de iteraciones de GRASP
#define RCL_SIZE 4        // Tamano de la lista RCL

//Local Search
#define LSBB_MAXIT 1000 // Cantidad maxima de iteraciones de local search

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


/*
  El empeoramiento de la solucion estrella se
  decide con la probabilidad de recosido simulado
*/
