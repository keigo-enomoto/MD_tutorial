
#ifndef INCLUDE_TEMP_H
#define INCLUDE_TEMP_H
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
double calc_temp(int npa, double *vl);
void norm_temp(int npa, double target_temp, double T, double *nvl);
void modify_temp(int npa, double target_temp, double *vl, double *nvl);
//------------------------------------------------
#endif
