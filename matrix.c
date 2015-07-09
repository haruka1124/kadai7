#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

void i_k_j_roop(double **a,double **b,double **c,int n){
  double begin,end;
  begin = get_time();

  int i,j,k;
  for (i = 0; i < n; i++)
    for (k = 0; k < n; k++)   
      for (j = 0; j < n; j++)
	c[i][j] += a[i][k] * b[k][j];
  end = get_time();
  printf("i-k-j-> ");
  printf("time: %.6lf sec\n", end - begin);
}

void k_i_j_roop(double **a,double **b,double **c,int n){
  double begin,end;
  begin = get_time();
  
  int i,j,k;
  for (k = 0; k < n; k++)   
    for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
	c[i][j] += a[i][k] * b[k][j];
  end = get_time();
  printf("k-i-j-> ");
  printf("time: %.6lf sec\n", end - begin);
}

void i_j_k_roop(double **a,double **b,double **c,int n){
  double begin,end;
  begin = get_time();
  
  int i,j,k;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < n; k++)   
	c[i][j] += a[i][k] * b[k][j];
  end = get_time();
  printf("i-j-k-> ");
  printf("time: %.6lf sec\n", end - begin);
}

void j_i_k_roop(double **a,double **b,double **c,int n){
  double begin,end;
  begin = get_time();
  
  int i,j,k;
  for (j = 0; j < n; j++)
    for (i = 0; i < n; i++)
      for (k = 0; k < n; k++)   
	c[i][j] += a[i][k] * b[k][j];
  end = get_time();
  printf("j-i-k-> ");
  printf("time: %.6lf sec\n", end - begin);
}


void k_j_i_roop(double **a,double **b,double **c,int n){
  double begin,end;
  begin = get_time();
  
  int i,j,k;
  for (k = 0; k < n; k++) 
    for (j = 0; j < n; j++)  
      for (i = 0; i < n; i++)
	c[i][j] += a[i][k] * b[k][j];
  end = get_time();
  printf("k-j-i-> ");
  printf("time: %.6lf sec\n", end - begin);
}

void j_k_i_roop(double **a,double **b,double **c,int n){
  double begin,end;
  begin = get_time();
  
  int i,j,k;
  for (j = 0; j < n; j++)
    for (k = 0; k < n; k++)
      for (i = 0; i < n; i++)   
	c[i][j] += a[i][k] * b[k][j];
  end = get_time();
  printf("j-k-i-> ");
  printf("time: %.6lf sec\n", end - begin);
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    printf("usage: %s N\n", argv[0]);
    return -1;
  }

  int n = atoi(argv[1]);
  double** a = malloc(sizeof(double *) * n); // Matrix A
  double** b = malloc(sizeof(double *) * n); // Matrix A
  double** c = malloc(sizeof(double *) * n); // Matrix A
 
  for (int m=0;m<n;m++) {
    a[m] = malloc(sizeof(double) * n);
    b[m] = malloc(sizeof(double) * n);
    c[m] = malloc(sizeof(double) * n);
  }  

  // Initialize the matrices to some values.
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      a[i][j] = i * n + j; // A[i][j]
      b[i][j] = j * n + i; // B[i][j]
      c[i][j] = 0; // C[i][j]
    }
  }
  
  i_k_j_roop(a,b,c,n);
  for(i = 0; i<n ; i++)
    c[i][j] = 0;
  k_i_j_roop(a,b,c,n);
  for(i = 0; i<n ; i++)
    c[i][j] = 0;
  i_j_k_roop(a,b,c,n);
  for(i = 0; i<n ; i++)
    c[i][j] = 0;
  j_i_k_roop(a,b,c,n);
  for(i = 0; i<n ; i++)
    c[i][j] = 0;
  k_j_i_roop(a,b,c,n);
  for(i = 0; i<n ; i++)
    c[i][j] = 0;
  j_k_i_roop(a,b,c,n);
  for(i = 0; i<n ; i++)
    c[i][j] = 0;

  for (i=0;i<n;i++) {
    free(a[i]);
    free(b[i]);
    free(c[i]);
  }
  free(a);
  free(b);
  free(c);
  return 0;
}
