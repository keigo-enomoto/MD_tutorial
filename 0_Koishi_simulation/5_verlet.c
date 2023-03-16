//
//  5_verlet.c
//
//
//  Created by Keigo Enomoto on 2022/3/7.
//  https://polymer.apphy.u-fukui.ac.jp/~koishi/lecture/md_program5/index.php
//

#include <stdio.h>
#include <math.h>


void verlet(double dt);
int main(void)
{

//0.3
//verlet(0.3);

//0.1
//verlet(0.1);

//0.05
verlet(0.05);


}


void verlet(double dt)
{
  int i = 0;
  double x,v,f;
  double t=0.0;


  x = -1.0;
  v = 0.0;
  f = cos(t);


  printf(" 時間    ベルレx   理論値x   ベルレv   理論値v    力\n");

  FILE *fp;

  fp = fopen("output/5_verlet.txt","w");

  for(i = 1; i < 360; i++){
    t = i*dt;
    v += f*dt*0.5;
    x += v*dt;
    f = cos(t);
    v += f*dt*0.5;
    // fprintf(fp,"%f %f\n",t,x);
    fprintf(fp,"%f %8.4f %8.4f %8.4f %8.4f %8.4f\n",t,x,-cos(t),v,sin(t),f);
  }

  fclose(fp);

}
