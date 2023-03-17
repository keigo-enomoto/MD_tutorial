
#include <stdio.h>
#include <stdlib.h>



// https://programming-place.net/ppp/contents/c/035.html
static void* xmalloc(size_t size)
{
    void* p = malloc( size );
    if( p == NULL ){
        fprintf(stderr,"\nout of memory allocating %lu bytes\n",(unsigned long)size);
        exit( EXIT_FAILURE );
    }
    return p;
}


void allocate_arrays(int npa, double **cd, double **vl, double **nvl, double **fc){

    *cd = (double *)xmalloc(3*npa*sizeof(double));
    *vl = (double *)xmalloc(3*npa*sizeof(double));
    *nvl = (double *)xmalloc(3*npa*sizeof(double));
    *fc = (double *)xmalloc(3*npa*sizeof(double));

}