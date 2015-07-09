#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define part_file 10
int num = 1 * 1000 * 1000 / sizeof(int); // initial block size

int comp(const void *a,const void *b){
  return *(int*)a - *(int*)b;
}

int main(int argc, char** argv){
  if (argc != 2) {
    printf("usage: %s <filename>\n", argv[0]);
    exit(1);
  }
  char* filename = argv[1];
  char outfile[50];
  char str1[] = "part";
  int i,j,file_num;
  size_t ret;
  int* buffer = (int*)malloc(num * sizeof(int));
  FILE* inputfp = fopen(filename, "rb");
  FILE* fp[2][part_file];

  if (inputfp == NULL) {
    printf("error: Cannot open file %s\n", filename);
    exit(1);
  }
  for(i=0;i<part_file;i++){
    sprintf(outfile,"%s_%d",str1,i);
    printf("open : %s\n",outfile);
    if((fp[0][i] = fopen(outfile,"wb+"))==NULL){
      printf("error: Can't open %s\n",outfile);
      exit(1);
    }
  }

  j = 0;
  while (1) {
    file_num = j % part_file;
    ret = fread(buffer, sizeof(int), num, inputfp);
    if (ret == 0)
      break;
    qsort(buffer,num,sizeof(int),comp);
    ret = fwrite(buffer, sizeof(int), num, fp[0][file_num]);
    if(ret !=num){
      printf("error: Failed in writing bytes to the file %s_%d\n", str1,file_num);
      exit(1);
    }
    j++;
  }
  free(buffer);
  fclose(inputfp);

  //++  merge  +++++++++++++++
  int length = 4 * num;
  char str2[] = "tmp";
  size_t ret1,ret2;
  int k;

  //open file
  for(i=0;i<part_file;i++){
    sprintf(outfile,"%s_%d",str2,i);
    outfile[strlen(outfile)]='\0';
    fp[1][i] = fopen(outfile, "wb+"); 
    if (fp[1][i] == NULL) {
      printf("error: Cannot open file %s\n", outfile);
      exit(1);
    }
  }
  
  //merge
  int* buffer1;
  int* buffer2;
  int bf_block, block = num;
  int readf = 0;
  int outf = 1;
  
  while(1){
    printf("read_file = %d, output_file = %d\n",readf,outf);
    printf("block_size = %d\n",block);
    bf_block = 2* block;
    buffer1 = (int*)malloc(block * sizeof(int));
    buffer2 = (int*)malloc(block * sizeof(int));
    buffer = (int*)malloc(bf_block * sizeof(int)); 
    
    for(int i=0;i<part_file;i++){
      fseek(fp[readf][i], 0, SEEK_SET);
      fseek(fp[outf][i], 0, SEEK_SET);
    }
    k =0; j=0;
    while(1){
      printf(" ++++ one_block ++++ \n");
      file_num = k  % part_file;   
      ret1 = fread(buffer1, sizeof(int), block, fp[readf][j]);
      ret2 = fread(buffer2, sizeof(int), block, fp[readf][j+1]);
      printf("ret1 = %zu, ret2 = %zu\n",ret1,ret2);
      if (ret1 == 0 || ret2 == 0){
	printf("k = %d, block = %d,%d, file_num = %d\n",k,j,j+1,file_num);
	printf(" ++++ finish_file ++++ \n");
	if(file_num!=0){
	  printf("Error_file_num is %d\n",file_num);
	  exit(1);
	}
	break;
      }
      for(int m=0,g=0;g<block;m=m+2,g++){
	buffer[m] = buffer1[g];
	buffer[m+1] = buffer2[g];
      }
      qsort(buffer,bf_block,sizeof(int),comp);
      printf("k = %d, block = %d,%d, file_num = %d\n",k,j,j+1,file_num);

      if((fwrite(buffer, sizeof(int), bf_block, fp[outf][file_num]))!=bf_block){
	printf("error: Failed in writing bytes to the file %s_%d\n",str1,file_num);
	exit(1);
      }
      j=(j+2) % part_file;
      k++;
      printf(" ++++++++++++++++ \n");
    }
    free(buffer1);
    free(buffer2);
    free(buffer);
    
    block = block*2;  //extend BLOCK
    if(block > length/2 + 1) 
      break;  
    outf = readf;
    readf = (readf+1) %2;
  }

  

  //Each of fp[outf]
  //  (fp[outf][0],fp[outf][1],..,fp[outf][part_file-1])
  //has sorted data.
  for(i=0;i<part_file;i++){
   fseek(fp[outf][i], 0, SEEK_SET);
    fclose(fp[readf][i]);
  }
 
  //open file
  bf_block =  4*num;
  int *sorted_buf = (int*)malloc( bf_block * sizeof(int));
  int *buffers[part_file];
  FILE* result;
  int min,index,pt[part_file]={0};
  int finished_file=0;

  //initialize
  for(i=0;i<part_file;i++){
    buffers[i] = (int*)malloc(num * sizeof(int));
    ret = fread(buffers[i],sizeof(int),num, fp[outf][i]);
    if(ret==0)
      printf("error: file%d has no data.\n",i);
  }
  if((result = fopen("result","wb")) == NULL){
    printf("error: Cannot open file\n");
    exit(1);
  }

  k = 0;
  while(finished_file<part_file){
    
    while(k<bf_block){
      min = buffers[0][pt[0]];
      index = 0;
      for(i=1;i<part_file;i++)
	if(min > buffers[i][pt[i]]){
	  min = buffers[i][pt[i]];
	  index = i;
	}
      pt[index]++;
      sorted_buf[k] = min;
      if(pt[index] >= num){
	pt[index] = 0;	  
	ret = fread(buffers[index],sizeof(int),num, fp[outf][index]);
	if(ret==0){
	  buffers[index][0] = INT_MAX;//dammy
	  finished_file++;
	  if(finished_file>=part_file)
	    break;
	}
      }
      k++;
    }
    ret = fwrite(sorted_buf,sizeof(int),k,result);
    k = 0;
  }

  free(sorted_buf);
  fclose(result);

  for(int i=0;i<part_file;i++){
    free(buffers[i]);
    fclose(fp[outf][i]);

    //remove _ file cyuukan_file
    sprintf(outfile,"%s_%d",str1,i);
    outfile[strlen(outfile)]='\0';
    remove(outfile); 
    sprintf(outfile,"%s_%d",str2,i);
    outfile[strlen(outfile)]='\0';
    remove(outfile); 
  }
}
