#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define SIZE 50

double do_crazy_computation(int i,int j);

int main(int argc, char **argv) {
  struct timeval start1, end1;
  struct timeval start2, end2;
  struct timeval start3, end3;

  double elapsed;
  double loadImbalance1;
  double loadImbalance2;
  double loadImbalanceFinal; 
  double mat[SIZE][SIZE];
  int i,j;



  omp_set_num_threads(2);
  gettimeofday(&start1, NULL);
  gettimeofday(&start2, NULL);
  gettimeofday(&start3, NULL);
#pragma omp parallel shared(mat) private(i,j)
{
  #pragma omp sections
  {
    #pragma omp section //top part of the array
    {
        for (i=0;i<SIZE/2;i++) { /* loop over the rows */
          for (j=0;j<SIZE;j++) {  /* loop over the columns */
            mat[i][j] = do_crazy_computation(i,j);
            fprintf(stderr,".");
          }
        }
          gettimeofday(&end2, NULL);
          loadImbalance1 = ((end2.tv_sec*1000000.0 + end2.tv_usec) -
                      (start2.tv_sec*1000000.0 + start2.tv_usec)) / 1000000.00;
    }
    #pragma omp section //bottom part of the array
    {
        for (i=SIZE/2;i<SIZE;i++) { /* loop over the rows */
          for (j=0;j<SIZE;j++) {  /* loop over the columns */
            mat[i][j] = do_crazy_computation(i,j);
            fprintf(stderr,".");
          }
        } 
          gettimeofday(&end3, NULL);
          loadImbalance2 = ((end3.tv_sec*1000000.0 + end3.tv_usec) -
                      (start3.tv_sec*1000000.0 + start3.tv_usec)) / 1000000.00;

    }
  }
}
  gettimeofday(&end1, NULL);
 
  elapsed = ((end1.tv_sec*1000000.0 + end1.tv_usec) -
            (start1.tv_sec*1000000.0 + start1.tv_usec)) / 1000000.00;

  loadImbalanceFinal = fabs(loadImbalance1-loadImbalance2);
  printf("Thread #0 execution time: %.2f sec\n",loadImbalance1);
          printf("Thread #1 execution time: %.2f sec\n",loadImbalance2);
  printf("Elapsed time: %.2f seconds\n",elapsed);
  printf("Load Imbalance: %.2f seconds\n", loadImbalanceFinal);

  exit(0);
}

/* Crazy Computation */
double do_crazy_computation(int x,int y) {
   int iter;
   double value=0.0;

   for (iter = 0; iter < 5*x*x*x+1 + y*y*y+1; iter++) {
     value +=  (cos(x*value) + sin(y*value));
   }
  return value;
}

