//
//  2_3_menshin.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program2/index.php
//


#include <stdio.h>
#include <math.h>

enum {
  NUM_FCC_PARTICLE = 4  // 面心立方格子の単位格子の内の粒子の数
};

void output_xyz(double *cd, int npa);
void writeData_inVtuFormat(double *cd,int N);

int main(int argc, char **argv){


    int nla = 2;                              // 面心立方格子の単位格子の数
    int npa = NUM_FCC_PARTICLE*nla*nla*nla;   // 全粒子数
    double side, side_unit;
    double density = 0.817657;

    int i=0;
    int ix, iy, iz;
    double cd[npa * 3];

    side_unit = pow(NUM_FCC_PARTICLE / density, 1./3.);
    side = side_unit*nla;

    for(iz = 0; iz < nla; iz++){
        for(iy = 0; iy < nla; iy++){
            for(ix = 0; ix < nla; ix++){
                cd[i]   = side_unit*ix;
                cd[i+1] = side_unit*iy;
                cd[i+2] = side_unit*iz;
                i += 3;
                cd[i]   = side_unit*(ix+0.5);
                cd[i+1] = side_unit*(iy+0.5);
                cd[i+2] = side_unit*iz;
                i += 3;
                cd[i]   = side_unit*(ix+0.5);
                cd[i+1] = side_unit*iy;
                cd[i+2] = side_unit*(iz+0.5);
                i += 3;
                cd[i]   = side_unit*ix;
                cd[i+1] = side_unit*(iy+0.5);
                cd[i+2] = side_unit*(iz+0.5);
                i += 3;
            }
        }
    }

    writeData_inVtuFormat(cd,npa);
    output_xyz(cd,npa);

    return 0;

}

/**********************
 *  output functions
***********************/

// Visualize using VMD
void output_xyz(double *cd, int npa){

    FILE *fp;
    int i;
    fp = fopen("2_3_mensin.xyz", "w");

    fprintf(fp, "%d \n", npa);
    fprintf(fp, "md_simulation at step 1\n");

    for (i=0; i<npa; i++){

        fprintf(fp,"%d %f %f %f \n", 1, cd[i*3], cd[i*3+1], cd[i*3+2]);

    }

    fclose(fp);

}


// Visualize using ParaView
void writeData_inVtuFormat(double *cd,int N){
  int i;
  FILE *fp;
  char fileName[1024]; // This is array to have the word of filename (fileName[0] = p, fileName[1] = a)
  sprintf(fileName, "2_3_menshin.vtu"); //sprintf is the function to write the word to array

  fp=fopen(fileName,"w");
  fprintf(fp,"<?xml version='1.0' encoding='UTF-8'?>\n");
  fprintf(fp,"<VTKFile xmlns='VTK' byte_order='LittleEndian' version='0.1' type='UnstructuredGrid'>\n");
  fprintf(fp,"<UnstructuredGrid>\n");
  fprintf(fp,"<Piece NumberOfCells='%d' NumberOfPoints='%d'>\n",N,N);
  fprintf(fp,"<Points>\n");
  fprintf(fp,"<DataArray NumberOfComponents='3' type='Float32' Name='Position' format='ascii'>\n");
  for(i=0;i<N;i++){
    fprintf(fp,"%lf %lf %lf\n",cd[i*3],cd[i*3+1],cd[i*3+2]);
  }
  fprintf(fp,"</DataArray>\n");
  fprintf(fp,"</Points>\n");
  fprintf(fp,"<PointData>\n");
  fprintf(fp,"<DataArray NumberOfComponents='1' type='Int32' Name='ParticleType' format='ascii'>\n");
  for(i=0;i<N;i++){
    fprintf(fp,"%d\n",1);
  }
  fprintf(fp,"</DataArray>\n");
  fprintf(fp,"</PointData>\n");
  fprintf(fp,"<Cells>\n");
  fprintf(fp,"<DataArray type='Int32' Name='connectivity' format='ascii'>\n");
  for(i=0;i<N;i++){
    fprintf(fp,"%d\n",i);
  }
  fprintf(fp,"</DataArray>\n");
  fprintf(fp,"<DataArray type='Int32' Name='offsets' format='ascii'>\n");
  for(i=0;i<N;i++){
    fprintf(fp,"%d\n",i+1);
  }
  //点(粒子)のデータなら'1'が入る
  fprintf(fp,"</DataArray>\n");
  fprintf(fp,"<DataArray type='UInt8' Name='types' format='ascii'>\n");
  for(i=0;i<N;i++){
    fprintf(fp,"1\n");
  }
  fprintf(fp,"</DataArray>\n");
  fprintf(fp,"</Cells>\n");
  fprintf(fp,"</Piece>\n");
  fprintf(fp,"</UnstructuredGrid>\n");
  fprintf(fp,"</VTKFile>\n");
  fclose(fp);
}
