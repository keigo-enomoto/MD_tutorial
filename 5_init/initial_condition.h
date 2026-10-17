
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
void init_position_FCC(int nla, double side_unit, double *cd);
void init_velocity(int seed, int npa, double *vl);
void write_configuration(char *file_name, int npa, double side, double *cd, double *vl);
void read_initial_configuration(char *file_name, int *npa, double *side, double **cd, double **vl, double **nvl, double **fc);

//------------------------------------------------
#endif

