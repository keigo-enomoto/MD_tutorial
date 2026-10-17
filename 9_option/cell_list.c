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
    力の計算 (セルリスト版, OpenMP 並列)
=====================*/

// 各粒子 i は「自分の力 fc[i] だけ」を更新する (j != i の全ての組を調べる)。
// 別のスレッドが同じ fc に書き込むことがないので、データ競合が起きない。
// 組を2回ずつ調べるので、ポテンシャルエネルギーは 0.5*phi を足す。
double calc_force_pot_LJ_cell(int npa, double *cd, double *fc, double side, double cutoff, int *head, int *next){

    int ncell = calc_ncell(side,cutoff);
    double pot_erg = 0.0;
    double sideh = 0.5 * side;
    double cut_off2 = cutoff * cutoff;

    // initialize ---------
    zero_force(npa,fc);
    make_cell_list(npa,cd,side,ncell,head,next);  // 連結リストへの挿入は逐次的なので並列化しない

    // calc force and potential -------------
    // ループの中で宣言した変数は、スレッドごとに別々に持たれる (private)
    #pragma omp parallel for collapse(3) reduction(+:pot_erg)
    for(int cz = 0; cz < ncell; cz++){
    for(int cy = 0; cy < ncell; cy++){
    for(int cx = 0; cx < ncell; cx++){
        // 周囲の27セル (自分を含む)。周期境界なので端の外側は反対側のセルに巻き戻す
        for(int ez = -1; ez <= 1; ez++){
        for(int ey = -1; ey <= 1; ey++){
        for(int ex = -1; ex <= 1; ex++){
            int nx = (cx + ex + ncell) % ncell;
            int ny = (cy + ey + ncell) % ncell;
            int nz = (cz + ez + ncell) % ncell;
            int c = nx + ny*ncell + nz*ncell*ncell;

            // セル内の粒子 i をリストでたどり、隣のセルの粒子 j との組を調べる
            for(int i = head[cx + cy*ncell + cz*ncell*ncell]; i != -1; i = next[i]){
                for(int j = head[c]; j != -1; j = next[j]){
                    if(j == i){ continue; }  // 自分自身は除く。j > i の制限はしない
                    double dd[3];
                    dd[0] = cd[3*i]   - cd[3*j];
                    dd[1] = cd[3*i+1] - cd[3*j+1];
                    dd[2] = cd[3*i+2] - cd[3*j+2];
                    if(dd[0] < -sideh){ dd[0] += side;} else if(dd[0] >  sideh){ dd[0] -= side;}
                    if(dd[1] < -sideh){ dd[1] += side;} else if(dd[1] >  sideh){ dd[1] -= side;}
                    if(dd[2] < -sideh){ dd[2] += side;} else if(dd[2] >  sideh){ dd[2] -= side;}
                    double r2 = dd[0]*dd[0] + dd[1]*dd[1] + dd[2]*dd[2];
                    if(r2 < cut_off2){
                        double r6 = r2*r2*r2;
                        double r12 = r6*r6;
                        double phi = 4.0*(1.0/(r12) - 1.0/(r6));
                        double dphi = 4.0*(12.0/(r12*r2) - 6.0/(r6*r2));
                        pot_erg += 0.5*phi;  // 組 (i,j) は j 側からも数えられるので半分
                        // 書き込むのは fc[i] だけ
                        fc[3*i]   += dphi*dd[0];
                        fc[3*i+1] += dphi*dd[1];
                        fc[3*i+2] += dphi*dd[2];
                    }
                }
            }
        }}}
    }}}

    return pot_erg;
}
