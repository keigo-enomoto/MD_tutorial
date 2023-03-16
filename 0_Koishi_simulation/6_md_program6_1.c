//
//  6_md_program6_1.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program6/index.php
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum {
  N = 4
};

void output_file(int np, double *cd, double side)
{
  int i;
  FILE *fp;
  char file_name[64];
  static int c = 0;
  sprintf(file_name,"output/cd%04d.cdv",c++);
  if((fp = fopen(file_name,"wt")) == NULL){
      fprintf(stderr,"file open error %s\n",file_name);
  }
  fprintf(fp,"' box_sx=0 box_sy=0 box_sz=0 box_ex=%f box_ey=%f box_ez=%f box_wt=0.01\n",
          side,side,side);
  for(i = 0; i < np*3; i += 3){
      fprintf(fp,"%d 1 %f %f %f\n",i/3,cd[i],cd[i+1],cd[i+2]);
  }
  fclose(fp);
}


int main()
{
  int i;
  int step,total_step;
  double side = 9;
  double cd[N*3],vl[N*3];
  double dt = 3.0;
  int np = N;

  cd[0] = 4.5; cd[1]  = 3.0; cd[2]  = 3.0;
  cd[3] = 4.5; cd[4]  = 3.0; cd[5]  = 6.0;
  cd[6] = 4.5; cd[7]  = 6.0; cd[8]  = 3.0;
  cd[9] = 4.5; cd[10] = 6.0; cd[11] = 6.0;
  srand(1);
  for(i = 0; i < np*3; i++){
      vl[i] = ((double)rand()/RAND_MAX-.5)*.1;
  }

  total_step = 100;
  for(step = 1; step <= total_step; step++){
      for(i = 0; i < np*3; i++){
          cd[i] += vl[i]*dt;
          // periodic boundary condition
          if(cd[i]>=side){
              cd[i]-=side;
          }
          else if(cd[i]<=0.0){
              cd[i]+=side;
          }
      }
      output_file(np,cd,side);
      for(i = 0; i < np*3; i += 3){
          printf("%d %d %f %f %f\n",step,i/3,cd[i],cd[i+1],cd[i+2]);
      }
  }
  return 0;
}
