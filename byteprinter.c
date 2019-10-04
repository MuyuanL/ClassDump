#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if (argc < 2 || (argc == 3 && strcmp(argv[1], "-16"))){
    fprintf(stderr, "syntax: classdump (-16)? <classname>\n");
    exit(1);
  }
  char* class_name = argc==2? argv[1] : argv[2];
  int lineSize = argc==2? 8 : 16;

  FILE *fp;
  fp = fopen(class_name, "r");
  if (fp == NULL) {
    fprintf(stderr, "Can't open output file %s!\n",
	    class_name);
    exit(1);
  }

  char c;
  char buf[32];
  int i;
  while(fgets(buf, lineSize+1, fp) != NULL){
    for(i = 0; i < lineSize; i ++){
      printf("%02X%c", buf[i]>=0xFFFFFF00?buf[i]-0xFFFFFF00:buf[i], buf[i]>=0xFFFFFF00?'*':' ');
    }
    printf(" | ");
    for(i = 0; i < lineSize; i ++){
      printf("%c ", (buf[i]>=32&&buf[i]<127)?buf[i]:'.');
    }
    if(lineSize < 10){
      printf(" | ");
      for(i = 0; i < lineSize; i ++){
	printf("%03u ", buf[i]>=0xFFFFFF00?buf[i]-0xFFFFFF00:buf[i]);
      }
    }
    printf("\n");
  }
  /*
  while((c = fgetc(fp))!= EOF){
    printf("%02X ", c);
  }
  */
  
}
