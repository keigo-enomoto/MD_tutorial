
#ifndef INCLUDE_UPDATE_H
#define INCLUDE_UPDATE_H
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
void update_vel_velret(int npa, double dt, double *nvl, double *fc);
void update_position(int npa, double dt, double *cd, double *nvl);
void apply_PBC(int npa, double side, double *cd);

//------------------------------------------------
#endif
