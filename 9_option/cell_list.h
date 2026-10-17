#ifndef INCLUDE_CELL_LIST_H
#define INCLUDE_CELL_LIST_H
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
int calc_ncell(double side, double cutoff);
void allocate_cell_list(int npa, double side, double cutoff, int **head, int **next);
void make_cell_list(int npa, double *cd, double side, int ncell, int *head, int *next);
double calc_force_pot_LJ_cell(int npa, double *cd, double *fc, double side, double cutoff, int *head, int *next);
//------------------------------------------------
#endif

