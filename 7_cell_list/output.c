#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>   // mkdir (POSIX)

#include "output.h"


// 出力ディレクトリ dir 内のファイル name を開く (開けなければ終了する)
// パスは snprintf で作る。sprintf と違い、バッファの大きさを超えて書き込まない
static FILE *open_in_dir(const char *dir, const char *name, const char *mode){

    char path[512];
    FILE *fp;

    snprintf(path,sizeof(path),"%s/%s",dir,name);
    fp = fopen(path,mode);
    if(fp == NULL){
        fprintf(stderr,"cannot open file : %s\n",path);
        exit(EXIT_FAILURE);
    }
    return fp;
}


// 出力ディレクトリを作る。すでにある場合は何もしない
void make_output_dir(const char *dir){

    if(mkdir(dir,0755) != 0 && errno != EEXIST){
        fprintf(stderr,"cannot make output directory : %s\n",dir);
        exit(EXIT_FAILURE);
    }
}


// 実行時のパラメータを input.txt と同じ形式で書き出す (そのまま input として再利用できる)
void write_parameters(const char *dir, simulation_parameters *parameters){

    FILE *fp = open_in_dir(dir,"parameters.txt","w");

    fprintf(fp,"h = %.17g\n",parameters->h);
    fprintf(fp,"target_temp = %.17g\n",parameters->target_temp);
    fprintf(fp,"finish_time = %.17g\n",parameters->finish_time);
    fprintf(fp,"norm_interval = %d\n",parameters->norm_interval);
    fprintf(fp,"output_interval = %d\n",parameters->output_interval);
    fprintf(fp,"init_file = %s\n",parameters->init_file);
    fprintf(fp,"output_dir = %s\n",parameters->output_dir);
    fprintf(fp,"snapshot_interval = %d\n",parameters->snapshot_interval);
    fprintf(fp,"cutoff = %.17g\n",parameters->cutoff);

    fclose(fp);
}


// 時系列データ energy.dat を開き、ヘッダを書く (閉じるのは呼び出し側)
FILE *open_energy_file(const char *dir){

    FILE *fp = open_in_dir(dir,"energy.dat","w");
    fprintf(fp,"# time potential kinetic total temperature\n");
    return fp;
}


void write_energy(FILE *fp, double time, double pot_erg, double knt_erg, double T){

    fprintf(fp,"%.6f %.6f %.6f %.6f %.6f\n",time,pot_erg,knt_erg,pot_erg+knt_erg,T);
}


// xyz 形式 (VMD, OVITO 用): 1行目が粒子数、2行目がコメント、以降は "元素名 x y z"
void write_xyz(const char *dir, int number, int npa, double time, double *cd){

    int i;
    char name[64];
    snprintf(name,sizeof(name),"snap_%05d.xyz",number);
    FILE *fp = open_in_dir(dir,name,"w");

    fprintf(fp,"%d\n",npa);
    fprintf(fp,"time = %.6f\n",time);
    for(i = 0; i < npa*3; i += 3){
        fprintf(fp,"Ar %.6f %.6f %.6f\n",cd[i],cd[i+1],cd[i+2]);
    }

    fclose(fp);
}


// VTU 形式 (ParaView 用): VTK XML の UnstructuredGrid を ASCII で書く
// 粒子を「点1個だけからなるセル (VTK_VERTEX = 1)」として表す
void write_vtu(const char *dir, int number, int npa, double *cd, double *vl){

    int i;
    char name[64];
    snprintf(name,sizeof(name),"particle_%05d.vtu",number);
    FILE *fp = open_in_dir(dir,name,"w");

    fprintf(fp,"<?xml version='1.0' encoding='UTF-8'?>\n");
    fprintf(fp,"<VTKFile xmlns='VTK' byte_order='LittleEndian' version='0.1' type='UnstructuredGrid'>\n");
    fprintf(fp,"<UnstructuredGrid>\n");
    fprintf(fp,"<Piece NumberOfCells='%d' NumberOfPoints='%d'>\n",npa,npa);

    fprintf(fp,"<Points>\n");
    fprintf(fp,"<DataArray NumberOfComponents='3' type='Float32' Name='Position' format='ascii'>\n");
    for(i = 0; i < npa*3; i += 3){
        fprintf(fp,"%f %f %f\n",cd[i],cd[i+1],cd[i+2]);
    }
    fprintf(fp,"</DataArray>\n");
    fprintf(fp,"</Points>\n");

    fprintf(fp,"<PointData>\n");
    fprintf(fp,"<DataArray NumberOfComponents='3' type='Float32' Name='Velocity' format='ascii'>\n");
    for(i = 0; i < npa*3; i += 3){
        fprintf(fp,"%f %f %f\n",vl[i],vl[i+1],vl[i+2]);
    }
    fprintf(fp,"</DataArray>\n");
    fprintf(fp,"</PointData>\n");

    fprintf(fp,"<Cells>\n");
    fprintf(fp,"<DataArray type='Int32' Name='connectivity' format='ascii'>\n");
    for(i = 0; i < npa; i++){ fprintf(fp,"%d\n",i); }
    fprintf(fp,"</DataArray>\n");
    fprintf(fp,"<DataArray type='Int32' Name='offsets' format='ascii'>\n");
    for(i = 0; i < npa; i++){ fprintf(fp,"%d\n",i+1); }
    fprintf(fp,"</DataArray>\n");
    fprintf(fp,"<DataArray type='UInt8' Name='types' format='ascii'>\n");
    for(i = 0; i < npa; i++){ fprintf(fp,"1\n"); }
    fprintf(fp,"</DataArray>\n");
    fprintf(fp,"</Cells>\n");

    fprintf(fp,"</Piece>\n");
    fprintf(fp,"</UnstructuredGrid>\n");
    fprintf(fp,"</VTKFile>\n");

    fclose(fp);
}
