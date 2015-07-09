#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  int kk = 0;
  if (argc == 3){
    kk = (atoi(argv[2]));
  }else if (argc != 2){
    printf("usage: %s <filename>\n", argv[0]);
    exit(1);
  }
  
  char* filename = argv[1];
  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("error: Cannot open file %s\n", filename);
    exit(1);
  }
  fseek(fp, 0, SEEK_END);
  size_t file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  int j = 0;
  int min,current = 0;
  int num = 1000 * 1000 / sizeof(int);
  int* buffer = (int*)malloc(num * sizeof(int));

  kk = num * kk;
  fseek(fp, kk * sizeof(int), SEEK_SET);
  printf("read from %d\n",kk);
  while (1) {
    size_t ret = fread(buffer, sizeof(int), num, fp);
    if (ret == 0)
      break;
    int i;
    if(j==0)min = buffer[0]; 
    for (i = 0; i < ret; i++) {
      if (buffer[i] < current) {
	j = j * num;
	printf("%d + %d \n",j,i);
        printf("Error: The array is not sorted. %d => %d is found.\n", current, buffer[i]);
        exit(1);
      }
      current = buffer[i];
      //      printf("i=%d, %d\n",i,current);
    }
    j++;
  }
  printf("min : %d \n",min);
  printf("max : %d \n",current);
  free(buffer);
  fclose(fp);
  if (current == 0) {
    printf("Error: The file exists, but the array is broken. The array contains only 0.\n");
    exit(1);
  }

  j = j * num;
  printf("%d  \n",j);
  printf("OK: The array is correctly sorted! File size is %.2lf MB\n", file_size / 1000.0 / 1000.0);
  return 0;
}
