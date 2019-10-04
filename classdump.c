#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char getchar(FILE* f){
  int i = fgetc(f);
  if (i == EOF){
    exit(1);
  }
  return (char)i;
}

short getshort(FILE* f){
  int i = fgetc(f);
  if (i == EOF){
    exit(1);
  }
  int j = fgetc(f);
  if (j == EOF){
    exit(1);
  }
  return (short)((i << 2) + j);
}

char* getnbytes(FILE* f, int n){
  char buf[n+1];
  if(fgets(buf, n+1, f) == NULL){
    exit(1);
  }
  return strdup(buf);
}

void printbytes(char* s, int len){
  for (int i = 0; i < len; i ++){
    printf("%02X%c", s[i] > 0xFFFFFF00? s[i] - 0xFFFFFF00 : s[i], s[i] > 0xFFFFFF00? '*':' ');
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  if (argc < 2){
    fprintf(stderr, "syntax: classdump <filepath>");
    exit(1);
  }

  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Can't open output file %s!\n",
	    argv[1]);
    exit(1);
  }

  /* get magic number*/
  char* magic = getnbytes(fp, 4);
  printf("magic number: \t\t");
  printbytes(magic, 4);

  /* get version number*/
  short minor_ver = getshort(fp);
  short major_ver = getshort(fp);
  printf("version number: \t");
  printf("%d.%d\n", major_ver, minor_ver);

  /* get constant pool info*/
  short cp_count = getshort(fp);
  printf("=====Constant Pool=====\n\t");
  printf("Size:%d\n\n", cp_count);

  //char* cpstart = getnbytes(fp, 10); /* not sure what the first 10 bytes in cp refers to */
  
  for(short i = 1; i < cp_count; i++){
    printf("\n--CP Entry %d--\nType: ", i);
    //get the tag
    char tag = getchar(fp);
    switch(tag){
    case 1:
      {
	printf("Utf8\n");
	short length = getshort(fp);
	char* data = getnbytes(fp, length);
	printf("Data: \"%s\"\n", data);
      }
      break;
    case 3:
      {
	printf("Integer\n");
	char* data = getnbytes(fp, 4);
      }
      break;
    case 4:
      {
	printf("Float\n");
	char* data = getnbytes(fp, 4);
      }
      break;
    case 5:
      {
	printf("Long\n");
	char* data = getnbytes(fp, 8);
      }
      break;
    case 6:
      {
	printf("Double\n");
	char* data = getnbytes(fp, 8);
      }
      break;
    case 7:
      {
      printf("Class\n");
      short name_index = getshort(fp);
      printf("name_index: #%d\n", name_index);
      }
      break;
    case 8:
      {
	printf("String\n");
	short data = getshort(fp);
	printf("string_index: #%d\n", data);
      }
      break;
    case 9:
      {
	printf("Fieldref\n");
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	printf("class_index: #%d\nname_and_type_index: #%d\n", class_index, name_and_type_index);
      }
      break;
    case 10:
      {
	printf("Methodref\n");
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	printf("class_index: #%d\nname_and_type_index: #%d\n", class_index, name_and_type_index);
      }

      break;
    case 11:
      {
	printf("InterfaceMethodref\n");
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	printf("class_index: #%d\nname_and_type_index: #%d\n", class_index, name_and_type_index);
      }
      break;
    case 12:
      {
	printf("NameAndType\n");
	short data1 = getshort(fp);
	short data2 = getshort(fp);
	printf("name_index: #%d\ndescriptor_index: #%d\n", data1, data2);
      }
      break;
    case 15:
      {
	printf("MethodHandle\n");
	char data1 = getchar(fp);
	short data2 = getshort(fp);
      }
      break;
    case 16:
      {
	printf("MethodType\n");
	short data = getshort(fp);
      }
      break;
    case 18:
      {
	printf("InvokeDynamic\n");
	short data1 = getshort(fp);
	short data2 = getshort(fp);
      }
      break;
    case 19:
      {
	printf("Module\n");
	short data = getshort(fp);
      }
      break;
    case 20:
      {
	printf("Package\n");
	short data = getshort(fp);
      }
      break;
    }
  }
  /*
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

  */
}
