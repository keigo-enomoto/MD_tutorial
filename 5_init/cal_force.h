
#ifndef INCLUDE_CAL_FORCE_H
#define INCLUDE_CAL_FORCE_H
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
void zero_force(int npa, double *fc);
double calc_force_pot_LJ(int npa, double *cd, double *fc, double side);
void check_sum_force(int npa, double *fc, double *sum_fc);
//------------------------------------------------
#endif

