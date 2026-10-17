#ifndef INCLUDE_OUTPUT_H
#define INCLUDE_OUTPUT_H

#include <stdio.h>
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
void make_output_dir(const char *dir);
void write_parameters(const char *dir, simulation_parameters *parameters);
FILE *open_energy_file(const char *dir);
void write_energy(FILE *fp, double time, double pot_erg, double knt_erg, double T);
void write_xyz(const char *dir, int number, int npa, double time, double *cd);
void write_vtu(const char *dir, int number, int npa, double *cd, double *vl);
//------------------------------------------------
#endif
