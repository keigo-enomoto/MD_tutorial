
#ifndef INCLUDE_UTILITY_H
#define INCLUDE_UTILITY_H

#include "common.h"
//------------------------------------------------

//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------
void allocate_arrays(int npa, double **cd, double **vl, double **nvl, double **fc);
simulation_parameters *read_parameters_from_inputfile(char **input_file);
//------------------------------------------------
#endif
