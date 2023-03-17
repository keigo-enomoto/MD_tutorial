
#ifndef INCLUDE_INITIAL_CONDITION_H
#define INCLUDE_INITIAL_CONDITION_H
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
void non_dimensionalize_parameters(double *n_density, double *n_h, double *n_target_temp);
void init_position_FCC(int nla, double side_unit, double *cd);
void init_velocity(int seed, int npa, double *vl);

//------------------------------------------------
#endif

