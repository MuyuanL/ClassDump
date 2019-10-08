#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum cp_tag{
	    CONSTANT_Utf8                   = 1,
	    CONSTANT_Integer                = 3,
	    CONSTANT_Float                  = 4,
	    CONSTANT_Long                   = 5,
	    CONSTANT_Double                 = 6,
	    CONSTANT_Class                  = 7,
	    CONSTANT_String                 = 8,
	    CONSTANT_Fieldref               = 9,
	    CONSTANT_Methodref              = 10,
	    CONSTANT_InterfaceMethodref     = 11,
	    CONSTANT_NameAndType            = 12,
	    CONSTANT_MethodHandle           = 15,
	    CONSTANT_MethodType             = 16,
	    CONSTANT_InvokeDynamic          = 18,
	    CONSTANT_Module                 = 19,
	    CONSTANT_Package                = 20
};

enum acc_flag{
	      ACC_PUBLIC = 0x0001,
	      ACC_PRIVATE = 0x0002,
	      ACC_PROTECTED = 0x0004,
	      ACC_STATIC = 0x0008,
	      ACC_FINAL = 0x0010,
	      ACC_SUPER = 0x0020,
	      ACC_VOLATILE = 0x0040,
	      ACC_TRANSIENT = 0x0080,
	      ACC_INTERFACE = 0x0200,
	      ACC_ABSTRACT = 0x0400,
	      ACC_SYNTHETIC = 0x1000,
	      ACC_ANNOTATION = 0x2000,
	      ACC_ENUM = 0x4000,
	      ACC_MODULE = 0x8000
};

/* structs for CP entries */

struct CONSTANT_Class_info {
    char tag;
    short name_index;
};


struct CONSTANT_Fieldref_info {
    char tag;
    short class_index;
    short name_and_type_index;
};

struct CONSTANT_Methodref_info {
    char tag;
    short class_index;
    short name_and_type_index;
};

struct CONSTANT_InterfaceMethodref_info {
    char tag;
    short class_index;
    short name_and_type_index;
};

struct CONSTANT_String_info {
    char tag;
    short string_index;
};

struct CONSTANT_Integer_info {
    char tag;
    int data;
};

struct CONSTANT_Float_info {
    char tag;
    float data;
};

struct CONSTANT_Long_info {
    char tag;
    long data;
};

struct CONSTANT_Double_info {
    char tag;
  double data;
};

struct CONSTANT_NameAndType_info {
    char tag;
    short name_index;
    short descriptor_index;
};

struct CONSTANT_Utf8_info {
    char tag;
    short length;
    char* data;
};

struct CONSTANT_MethodHandle_info {
    char tag;
    char reference_kind;
    short reference_index;
};

struct CONSTANT_MethodType_info {
    char tag;
    short descriptor_index;
};

struct CONSTANT_InvokeDynamic_info {
    char tag;
    short bootstrap_method_attr_index;
    short name_and_type_index;
};


struct CONSTANT_Module_info {
    char tag;
    short name_index;
};

struct CONSTANT_Package_info {
    char tag;
    short name_index;
};

/**
 * help functions for file reading
 */

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

/**
 * help function to print multiple bytes
 */
void printbytes(char* s, int len){
  for (int i = 0; i < len; i ++){
    printf("%02X%c", s[i] > 0xFFFFFF00? s[i] - 0xFFFFFF00 : s[i], s[i] > 0xFFFFFF00? '*':' ');
  }
  printf("\n");
}

/**
 * help function to check access flags
 */
void access_flag_check(short accflg){
  if(accflg & ACC_PUBLIC){
    printf("ACC_PUBLIC\t");
  }
  if(accflg & ACC_PRIVATE){
    printf("ACC_PRIVATE\t");
  }
  if(accflg & ACC_PROTECTED){
    printf("ACC_PROTECTED\t");
  }
  if(accflg & ACC_STATIC){
    printf("ACC_STATIC\t");
  }
  if(accflg & ACC_FINAL){
    printf("ACC_FINAL\t");
  }
  if(accflg & ACC_VOLATILE){
    printf("ACC_VOLATILE\t");
  }
  if(accflg & ACC_TRANSIENT){
    printf("ACC_TRANSIENT\t");
  }
  if(accflg & ACC_SUPER){
    printf("ACC_SUPER\t");
  }
  if(accflg & ACC_INTERFACE){
    printf("ACC_INTERFACE\t");
  }
  if(accflg & ACC_ABSTRACT){
    printf("ACC_ABSTRACT\t");
  }
  if(accflg & ACC_SYNTHETIC){
    printf("ACC_SYNTHETIC\t");
  }
  if(accflg & ACC_ANNOTATION){
    printf("ACC_ANNOTATION\t");
  }
  if(accflg & ACC_ENUM){
    printf("ACC_ENUM\t");
  }
  if(accflg & ACC_MODULE){
    printf("ACC_MODULE\t");
  }
  printf("\n");
}

void parse_attr(FILE *f){

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
  printf("Size:%d\n", cp_count);

  void** cp = (void**) malloc(sizeof(void*) * cp_count);
  
  for(short i = 1; i < cp_count; i++){
    printf("\n--CP Entry %d--\nType: ", i);
    //get the tag
    char tag = getchar(fp);
    switch(tag){
    case CONSTANT_Utf8:
      {
	printf("Utf8\n");
	short length = getshort(fp);
	char* data = getnbytes(fp, length);
	printf("Data: \"%s\"\n", data);
      }
      break;
    case CONSTANT_Integer:
      {
	printf("Integer\n");
	char* data = getnbytes(fp, 4);
      }
      break;
    case CONSTANT_Float:
      {
	printf("Float\n");
	char* data = getnbytes(fp, 4);
      }
      break;
    case CONSTANT_Long:
      {
	printf("Long\n");
	char* data = getnbytes(fp, 8);
      }
      break;
    case CONSTANT_Double:
      {
	printf("Double\n");
	char* data = getnbytes(fp, 8);
      }
      break;
    case CONSTANT_Class:
      {
      printf("Class\n");
      short name_index = getshort(fp);
      printf("name_index: #%d\n", name_index);
      }
      break;
    case CONSTANT_String:
      {
	printf("String\n");
	short data = getshort(fp);
	printf("string_index: #%d\n", data);
      }
      break;
    case CONSTANT_Fieldref:
      {
	printf("Fieldref\n");
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	printf("class_index: #%d\nname_and_type_index: #%d\n", class_index, name_and_type_index);
      }
      break;
    case CONSTANT_Methodref:
      {
	printf("Methodref\n");
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	printf("class_index: #%d\nname_and_type_index: #%d\n", class_index, name_and_type_index);
      }

      break;
    case CONSTANT_InterfaceMethodref:
      {
	printf("InterfaceMethodref\n");
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	printf("class_index: #%d\nname_and_type_index: #%d\n", class_index, name_and_type_index);
      }
      break;
    case CONSTANT_NameAndType:
      {
	printf("NameAndType\n");
	short data1 = getshort(fp);
	short data2 = getshort(fp);
	printf("name_index: #%d\ndescriptor_index: #%d\n", data1, data2);
      }
      break;
    case CONSTANT_MethodHandle:
      {
	printf("MethodHandle\n");
	char data1 = getchar(fp);
	short data2 = getshort(fp);
      }
      break;
    case CONSTANT_MethodType:
      {
	printf("MethodType\n");
	short data = getshort(fp);
      }
      break;
    case CONSTANT_InvokeDynamic:
      {
	printf("InvokeDynamic\n");
	short data1 = getshort(fp);
	short data2 = getshort(fp);
      }
      break;
    case CONSTANT_Module:
      {
	printf("Module\n");
	short data = getshort(fp);
      }
      break;
    case CONSTANT_Package:
      {
	printf("Package\n");
	short data = getshort(fp);
      }
      break;
    }
  }
  printf("=====END OF CP=====\n\n");
  
  /* access flag */
  short access_flag = getshort(fp);
  printf("access_flag:\t\t0x%04X\n  ", access_flag);
  access_flag_check(access_flag);

  /* this class */
  short this_class = getshort(fp);
  printf("this_class:\t\t#%d\n", this_class);
  
  /* super class */
  short super_class = getshort(fp);
  printf("super_class:\t\t#%d\n", super_class);

  /* interface */
  short interfaces_count = getshort(fp);
  printf("interfaces_count:\t%d\n", interfaces_count);
  if(interfaces_count > 0){ printf("  "); }
  for (short i = 0; i < interfaces_count; i ++){
    short i_index = getshort(fp);
    printf("#%d\t", i_index);
    if(! i % 5){
      printf("\n");
    }
  }

  /* fields */
  short fields_count = getshort(fp);
  printf("fields_count:\t%d\n", fields_count);
  for (short i = 0; i < fields_count; i ++){
    printf("-----field_info %d-----\n", i);
    short f_acc_flag = getshort(fp);
    printf("access_flag:\t\t0x%04X\n", f_acc_flag);
    if(f_acc_flag){ printf("  ");}
    access_flag_check(access_flag);

    short f_name = getshort(fp);
    printf("name_index:\t\t#%d\n", f_name);

    short f_descriptor = getshort(fp);
    printf("descriptor_index:\t#%d\n", f_descriptor);

    short f_att_count = getshort(fp);
    printf("f_att_coun:\t%d\n", f_att_count);
    if(f_att_count){
      for(short j = 0; j < f_att_count; j++){
	parse_attr(fp);
      }
    }
    printf("\n");
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
