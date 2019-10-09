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
  return (short)((i << 8) + j);
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


char* get_cp_utf8(void** cp, short index){
  return ((struct CONSTANT_Utf8_info *)(cp[index]))->data;
}

void parse_descriptor(char* str){
  if(str[0] == '('){
    //method descriptor
    printf("Paramters: ");
    if(str[1] == ')'){
      printf("None\n");
    }
    else{
      
    }
  }
  else{
    //field descriptor
    char* e = str;
    printf("<");
    while(e){
      switch(*e){
      case 'L': {
	printf("Object:%s>\n", e+2);
      }
	return;
	break;
      case '[': {
	printf("array of ");
	e++;
	continue;
	break;
      }
      case 'B':
	{
	  printf("byte>\n");
	}
	return;
	break;
      case 'C':
	{
	  printf("char>\n");
	}
	return;
	break;
      case 'D':
	{
	  printf("double>\n");
	}
	return;
	break;
      case 'F':
	{
	  printf("float>\n");
	}
	return;
	break;
      case 'I':
	{
	  printf("int>\n");
	}
	return;
	break;
      case 'J':
	{
	  printf("long>\n");
	}
	return;
	break;
      case 'S':
	{
	  printf("short>\n");
	}
	return;
	break;
      case 'Z':
	{
	  printf("boolean>\n");
	}
	return;
	break;
      }
      return;
    }
  }
}

void print_cp(void** cp, short len){
  for (int i = 1; i < len; i ++){
    printf("\n+++++ CP_ENTRY %d +++++\n", i);
    void * v_ptr = cp[i];
    switch (((char*)v_ptr)[0]){
      case CONSTANT_Utf8:
      {	
	struct CONSTANT_Utf8_info * e
	  = (struct CONSTANT_Utf8_info *)v_ptr;
	
	printf("Tag: Utf8\n");
	printf("Data: \"%s\"\n", e->data);
      }
      break;
    case CONSTANT_Integer:
      {
	printf("Tag: Integer\n");
	struct CONSTANT_Integer_info * e
	  = (struct CONSTANT_Integer_info *)v_ptr;
	printf("Data: %d\n", e->data);
      }
      break;
    case CONSTANT_Float:
      {
	printf("Tag: Float\n");
	struct CONSTANT_Float_info * e
	  = (struct CONSTANT_Float_info *)v_ptr;
	
	printf("Data: %f\n", e->data);
      }
      break;
    case CONSTANT_Long:
      {
	printf("Tag: Long\n");
        
	struct CONSTANT_Long_info * e
	  = (struct CONSTANT_Long_info *)v_ptr;
	
	printf("Data: %ld\n", e->data);
      }
      break;
    case CONSTANT_Double:
      {
	printf("Tag: Double\n");
	struct CONSTANT_Double_info * e
	  = (struct CONSTANT_Double_info *)v_ptr;
        printf("Data: %lf\n", e->data);
      }
      break;
    case CONSTANT_Class:
      {
	printf("Tag: Class\n");

	struct CONSTANT_Class_info * e
	  = (struct CONSTANT_Class_info *)v_ptr;
	printf("name_index: #%d\t<%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
      }
      break;
    case CONSTANT_String:
      {
	printf("Tag: String\n");

	struct CONSTANT_String_info * e
	  = (struct CONSTANT_String_info *)v_ptr;
	printf("string_index: #%d\t<%s>\n", e->string_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->string_index]))->data);
      }
      break;
    case CONSTANT_Fieldref:
      {
	printf("Tag: Fieldref\n");
	struct CONSTANT_Fieldref_info * e
	  = (struct CONSTANT_Fieldref_info *)v_ptr;

	short class_name_index = ((struct CONSTANT_Class_info *)(cp[e->class_index]))->name_index;
	printf("class_index: #%d\t<Class %s>\n", e->class_index,
	       ((struct CONSTANT_Utf8_info *)(cp[class_name_index]))->data);

	struct CONSTANT_NameAndType_info *nt_info = (struct CONSTANT_NameAndType_info *)(cp[e->name_and_type_index]);
	printf("name_and_type_index: #%d\t<Name:%s, Type:%s>\t", e->name_and_type_index,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->name_index]))->data,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
	parse_descriptor(((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
	
      }
      break;
    case CONSTANT_Methodref:
      {
	printf("Tag: Methodref\n");
	struct CONSTANT_Methodref_info * e
	  = (struct CONSTANT_Methodref_info *)v_ptr;

	short class_name_index = ((struct CONSTANT_Class_info *)(cp[e->class_index]))->name_index;
	printf("class_index: #%d\t<Class %s>\n", e->class_index,
	       ((struct CONSTANT_Utf8_info *)(cp[class_name_index]))->data);
	
	struct CONSTANT_NameAndType_info *nt_info = (struct CONSTANT_NameAndType_info *)(cp[e->name_and_type_index]);
	printf("name_and_type_index: #%d\t<Name:%s, Type:%s>\n", e->name_and_type_index,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->name_index]))->data,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
      }

      break;
    case CONSTANT_InterfaceMethodref:
      {
	printf("Tag: InterfaceMethodref\n");
	struct CONSTANT_InterfaceMethodref_info * e
	  = (struct CONSTANT_InterfaceMethodref_info *)v_ptr;

	short class_name_index = ((struct CONSTANT_Class_info *)(cp[e->class_index]))->name_index;
	printf("class_index: #%d\t<Class %s>\n", e->class_index,
	       ((struct CONSTANT_Utf8_info *)(cp[class_name_index]))->data);
	
	struct CONSTANT_NameAndType_info *nt_info = (struct CONSTANT_NameAndType_info *)(cp[e->name_and_type_index]);
	printf("name_and_type_index: #%d\t<Name:%s, Type:%s>\n", e->name_and_type_index,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->name_index]))->data,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
      }
      break;
    case CONSTANT_NameAndType:
      {
	printf("Tag: NameAndType\n");

	struct CONSTANT_NameAndType_info * e
	  = (struct CONSTANT_NameAndType_info *)v_ptr;

	printf("name_index: #%d\t<%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
	
	printf("descriptor_index: #%d\t<%s>\n", e->descriptor_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->descriptor_index]))->data);
      }
      break;
    case CONSTANT_MethodHandle:
      {
	printf("Tag: MethodHandle\n");
	
	struct CONSTANT_MethodHandle_info * e
	  = (struct CONSTANT_MethodHandle_info *)v_ptr;
      }
      break;
    case CONSTANT_MethodType:
      {
	printf("Tag: MethodType\n");
	struct CONSTANT_MethodType_info * e
	  = (struct CONSTANT_MethodType_info *)v_ptr;
	printf("descriptor_index: #%d\t<%s>\n", e->descriptor_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->descriptor_index]))->data);
      }
      break;
    case CONSTANT_InvokeDynamic:
      {
	printf("Tag: InvokeDynamic\n");
	struct CONSTANT_InvokeDynamic_info * e
	  = (struct CONSTANT_InvokeDynamic_info *)v_ptr;
      }
      break;
    case CONSTANT_Module:
      {
	printf("Tag: Module\n");
	
	struct CONSTANT_Module_info * e
	  = (struct CONSTANT_Module_info *)v_ptr;
	printf("name_index: #%d\t<%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
      }
      break;
    case CONSTANT_Package:
      {
	printf("Tag: Package\n");
	struct CONSTANT_Package_info * e
	  = (struct CONSTANT_Package_info *)v_ptr;
	printf("name_index: #%d\t<%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
      }
      break;
    }
  }
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
  //printf("\n");
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
    //get the tag
    char tag = getchar(fp);
    switch(tag){
    case CONSTANT_Utf8:
      {
	short length = getshort(fp);
	char* data = getnbytes(fp, length);
	struct CONSTANT_Utf8_info * e
	  = (struct CONSTANT_Utf8_info *)malloc(sizeof(struct CONSTANT_Utf8_info));
	e->tag = tag;
	e->length = length;
	e->data = strdup(data);
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Integer:
      {
	char* bytes_int = getnbytes(fp, 4);
	int data = (((int)bytes_int[0]) << 24) + (((int)bytes_int[1]) << 16) + (((int)bytes_int[2]) << 8) + ((int)bytes_int[3]);

	struct CONSTANT_Integer_info * e
	  = (struct CONSTANT_Integer_info *)malloc(sizeof(struct CONSTANT_Integer_info));
	e->tag = tag;
	e->data = data;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Float:
      {
	char* bytes_int = getnbytes(fp, 4);
	int int_data = (((int)bytes_int[0]) << 24) + (((int)bytes_int[1]) << 16)
	  + (((int)bytes_int[2]) << 8) + ((int)bytes_int[3]);
	union{
	  int x;
	  float y;
	}temp;
	temp.x = int_data;
	float data = temp.y;
	
	struct CONSTANT_Float_info * e
	  = (struct CONSTANT_Float_info *)malloc(sizeof(struct CONSTANT_Float_info));
	e->tag = tag;
	e->data = data;
	cp[i] = (void*)e;
	
      }
      break;
    case CONSTANT_Long:
      {
	char* bytes_long = getnbytes(fp, 8);
	long data = 0;
	for (int b = 0; b < 8; b ++){
	  data += ((long)bytes_long[i]) << (56 - b*8);
	};
	
	struct CONSTANT_Long_info * e
	  = (struct CONSTANT_Long_info *)malloc(sizeof(struct CONSTANT_Long_info));
	e->tag = tag;
	e->data = data;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Double:
      {
	char* bytes_long = getnbytes(fp, 8);
	long l_data = 0;
	for (int b = 0; b < 8; b ++){
	  l_data += ((long)bytes_long[i]) << (56 - b*8);
	}
	union{
	  long x;
	  double y;
	}temp;
	temp.x = l_data;
        double data = temp.y;
	
	struct CONSTANT_Double_info * e
	  = (struct CONSTANT_Double_info *)malloc(sizeof(struct CONSTANT_Double_info));
	e->tag = tag;
	e->data = data;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Class:
      {
	short name_index = getshort(fp);

	struct CONSTANT_Class_info * e
	  = (struct CONSTANT_Class_info *)malloc(sizeof(struct CONSTANT_Class_info));
	e->tag = tag;
	e->name_index = name_index;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_String:
      {
	short data = getshort(fp);

	struct CONSTANT_String_info * e
	  = (struct CONSTANT_String_info *)malloc(sizeof(struct CONSTANT_String_info));
	e->tag = tag;
	e->string_index = data;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Fieldref:
      {
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	struct CONSTANT_Fieldref_info * e
	  = (struct CONSTANT_Fieldref_info *)malloc(sizeof(struct CONSTANT_Fieldref_info));
	e->tag = tag;
	e->class_index = class_index;
	e->name_and_type_index = name_and_type_index;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Methodref:
      {
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	struct CONSTANT_Methodref_info * e
	  = (struct CONSTANT_Methodref_info *)malloc(sizeof(struct CONSTANT_Methodref_info));
	e->tag = tag;
	e->class_index = class_index;
	e->name_and_type_index = name_and_type_index;
	cp[i] = (void*)e;
      }

      break;
    case CONSTANT_InterfaceMethodref:
      {
	short class_index = getshort(fp);
	short name_and_type_index = getshort(fp);
	struct CONSTANT_InterfaceMethodref_info * e
	  = (struct CONSTANT_InterfaceMethodref_info *)malloc(sizeof(struct CONSTANT_InterfaceMethodref_info));
	e->tag = tag;
	e->class_index = class_index;
	e->name_and_type_index = name_and_type_index;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_NameAndType:
      {
	short data1 = getshort(fp);
	short data2 = getshort(fp);
	struct CONSTANT_NameAndType_info * e
	  = (struct CONSTANT_NameAndType_info *)malloc(sizeof(struct CONSTANT_NameAndType_info));
	e->tag = tag;
	e->name_index = data1;
	e->descriptor_index = data2;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_MethodHandle:
      {
	char data1 = getchar(fp);
	short data2 = getshort(fp);
	
	struct CONSTANT_MethodHandle_info * e
	  = (struct CONSTANT_MethodHandle_info *)malloc(sizeof(struct CONSTANT_MethodHandle_info));
	e->tag = tag;
	e->reference_kind = data1;
	e->reference_index = data2;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_MethodType:
      {
	short data = getshort(fp);
	
	struct CONSTANT_MethodType_info * e
	  = (struct CONSTANT_MethodType_info *)malloc(sizeof(struct CONSTANT_MethodType_info));
	e->tag = tag;
	e->descriptor_index = data;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_InvokeDynamic:
      {
	short data1 = getshort(fp);
	short data2 = getshort(fp);

	struct CONSTANT_InvokeDynamic_info * e
	  = (struct CONSTANT_InvokeDynamic_info *)malloc(sizeof(struct CONSTANT_InvokeDynamic_info));
	e->tag = tag;
	e->bootstrap_method_attr_index = data1;
	e->name_and_type_index = data2;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Module:
      {
	short data = getshort(fp);
	
	struct CONSTANT_Module_info * e
	  = (struct CONSTANT_Module_info *)malloc(sizeof(struct CONSTANT_Module_info));
	e->tag = tag;
	e->name_index = data;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Package:
      {
	short data = getshort(fp);
	
	struct CONSTANT_Package_info * e
	  = (struct CONSTANT_Package_info *)malloc(sizeof(struct CONSTANT_Package_info));
	e->tag = tag;
	e->name_index = data;
	cp[i] = (void*)e;
      }
      break;
    }
  }

  print_cp(cp, cp_count);
  printf("=====END OF CP=====\n\n");
  
  /* access flag */
  short access_flag = getshort(fp);
  printf("access_flag:\t\t0x%04X\n  ", access_flag);
  access_flag_check(access_flag);
  printf("\n");

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
  printf("fields_count:\t\t%d\n", fields_count);
  for (short i = 0; i < fields_count; i ++){
    printf("-----field_info %d-----\n", i);
    short f_acc_flag = getshort(fp);
    printf("access_flag:\t\t0x%04X\n", f_acc_flag);
    if(f_acc_flag){
      printf("\t");
      access_flag_check(access_flag);
      printf("\n");
    }
    

    short f_name = getshort(fp);
    printf("name_index:\t\t#%d\n", f_name);
    printf("  <name: %s>\n", get_cp_utf8(cp, f_name));

    short f_descriptor = getshort(fp);
    printf("descriptor_index:\t#%d\n  ", f_descriptor);
    parse_descriptor(get_cp_utf8(cp, f_descriptor));

    short f_att_count = getshort(fp);
    printf("f_att_coun:\t\t%d\n", f_att_count);
    if(f_att_count){
      for(short j = 0; j < f_att_count; j++){
	parse_attr(fp);
      }
    }
    printf("\n");
  }

  
}
