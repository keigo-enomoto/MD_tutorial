//
//  cell_list.c
//  セルリストを使った力の計算
//  ref : Allen & Tildesley, Computer Simulation of Liquids, Sec. 5.3
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cal_force.h"
#include "cell_list.h"
#include "utility.h"


/*====================
    セルの数
=====================*/

// 1辺あたりのセルの数。セルの長さ side/ncell がカットオフ以上になるように切り捨てる
int calc_ncell(double side, double cutoff){

    int ncell = (int)(side/cutoff);
    if(ncell < 3){
        // 3未満だと「周囲27セル」の中に同じセルが重複して入り、同じ組を2回数えてしまう
        fprintf(stderr,"box is too small for cell list : side = %f, cutoff = %f (ncell = %d < 3)\n",side,cutoff,ncell);
        exit(EXIT_FAILURE);
    }
    return ncell;
}


// head, next の配列を確保する
void allocate_cell_list(int npa, double side, double cutoff, int **head, int **next){

    int ncell = calc_ncell(side,cutoff);

    *head = (int *)xmalloc(ncell*ncell*ncell*sizeof(int));
    *next = (int *)xmalloc(npa*sizeof(int));
}


/*====================
    セルリストの作成
=====================*/

// head[c] : セル c の先頭の粒子の番号 (空なら -1)
// next[i] : 粒子 i と同じセルの次の粒子の番号 (最後なら -1)
void make_cell_list(int npa, double *cd, double side, int ncell, int *head, int *next){

    int i,c;
    int ix,iy,iz;
    double cell_len = side / ncell;

    for(c = 0; c < ncell*ncell*ncell; c++){ head[c] = -1; }

    for(i = 0; i < npa; i++){
        ix = (int)(cd[3*i]  /cell_len);
        iy = (int)(cd[3*i+1]/cell_len);
        iz = (int)(cd[3*i+2]/cell_len);
        // 座標がちょうど side のとき ix == ncell になってしまうので、最後のセルに入れる
        if(ix >= ncell){ ix = ncell - 1; }
        if(iy >= ncell){ iy = ncell - 1; }
        if(iz >= ncell){ iz = ncell - 1; }
        c = ix + iy*ncell + iz*ncell*ncell;

        next[i] = head[c];  // 今までの先頭を i の次にして、
        head[c] = i;        // i を新しい先頭にする
    }
}


/*====================
    力の計算 (セルリスト版)
=====================*/

double calc_force_pot_LJ_cell(int npa, double *cd, double *fc, double side, double cutoff, int *head, int *next){

    int i,j,c;
    int cx,cy,cz;   // 注目しているセル
    int nx,ny,nz;   // 隣のセル
    int ex,ey,ez;   // 隣のセルの相対位置 (-1, 0, 1)
    int ncell = calc_ncell(side,cutoff);
    double pot_erg;
    double r2,r6,r12;
    double dphi,phi;
    double sideh = 0.5 * side;
    double cut_off2 = cutoff * cutoff;

    double dd[3] = {0.0};  // distance
    double df[3] = {0.0};  // force

    // initialize ---------
    pot_erg = 0;
    zero_force(npa,fc);
    make_cell_list(npa,cd,side,ncell,head,next);

    // calc force and potential -------------
    for(cz = 0; cz < ncell; cz++){
    for(cy = 0; cy < ncell; cy++){
    for(cx = 0; cx < ncell; cx++){
        // 周囲の27セル (自分を含む)。周期境界なので端の外側は反対側のセルに巻き戻す
        for(ez = -1; ez <= 1; ez++){
        for(ey = -1; ey <= 1; ey++){
        for(ex = -1; ex <= 1; ex++){
            nx = (cx + ex + ncell) % ncell;
            ny = (cy + ey + ncell) % ncell;
            nz = (cz + ez + ncell) % ncell;

            // セル内の粒子 i をリストでたどり、隣のセルの粒子 j との組を調べる
            for(i = head[cx + cy*ncell + cz*ncell*ncell]; i != -1; i = next[i]){
                c = nx + ny*ncell + nz*ncell*ncell;
                for(j = head[c]; j != -1; j = next[j]){
                    if(j <= i){ continue; }  // 総当たりと同じく j > i の組だけ (作用反作用で両方更新)
                    dd[0] = cd[3*i]   - cd[3*j];
                    dd[1] = cd[3*i+1] - cd[3*j+1];
                    dd[2] = cd[3*i+2] - cd[3*j+2];
                    if(dd[0] < -sideh){ dd[0] += side;} else if(dd[0] >  sideh){ dd[0] -= side;}
                    if(dd[1] < -sideh){ dd[1] += side;} else if(dd[1] >  sideh){ dd[1] -= side;}
                    if(dd[2] < -sideh){ dd[2] += side;} else if(dd[2] >  sideh){ dd[2] -= side;}
                    r2  = dd[0]*dd[0] + dd[1]*dd[1] + dd[2]*dd[2];
                    if(r2 < cut_off2){
                        r6 = r2*r2*r2;
                        r12 = r6*r6;
                        phi = 4.0*(1.0/(r12) - 1.0/(r6));
                        dphi = 4.0*(12.0/(r12*r2) - 6.0/(r6*r2));
                        pot_erg += phi;
                        df[0] = dphi*dd[0]; df[1] = dphi*dd[1]; df[2] = dphi*dd[2];
                        fc[3*i] += df[0];   fc[3*i+1] += df[1]; fc[3*i+2] += df[2];
                        fc[3*j] -= df[0];   fc[3*j+1] -= df[1]; fc[3*j+2] -= df[2];
                    }
                }
            }
        }}}
    }}}

    return pot_erg;
}
