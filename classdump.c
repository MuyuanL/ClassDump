#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "classdump.h"
/**
 * help functions for file reading
 */

char getachar(FILE* f){
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
  
  //fprintf(stderr,"+ len1:0x%02X\tlen2:0x%02X\tlen:0x%02X\n", i,j,((i << 8) | j));
  return (short)((i << 8) | j);
}

int getint(FILE* f){
  int i = getshort(f);
  int j = getshort(f);
  return ((i << 16) | j);
}

char* getnbytes(FILE* f, int n){
  char buf[n+1];
  
  int i = 0;
  while(i < n){
    buf[i] = fgetc(f);
    if(buf[i] == EOF) {exit(1);}
    //printf("#%d:0x%X\n", i, buf[i]);
    i ++;
  }
  buf[i] = 0;
  
  /*
  if(fgets(buf, n+1, f) == NULL){
    exit(1);
  }

  printbytes(buf, n);
  */
  char* ret = (char*)malloc(sizeof(char) * (n+1));
  memcpy(ret, buf, sizeof(char)*n);
  ret[n] = 0;
  return ret;
  //return strndup(buf, n);
}

/**
 * help function to print multiple bytes
 */
void printbytes(char* s, int len){
  for (int i = 0; i < len; i ++){
    printf("%02X%c", s[i] > 0xFFFFFF00? s[i] - 0xFFFFFF00 : s[i], s[i] > 0xFFFFFF00? '*':' ');
    if (!((i + 1)%8)){
      printf("\n");
    }
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
	
	printf("+ Tag: Utf8\n");
	printf("+ Data: \"%s\"\n", e->data);
      }
      break;
    case CONSTANT_Integer:
      {
	printf("+ Tag: Integer\n");
	struct CONSTANT_Integer_info * e
	  = (struct CONSTANT_Integer_info *)v_ptr;
	printf("+ Data: %d\n", e->data);
      }
      break;
    case CONSTANT_Float:
      {
	printf("+ Tag: Float\n");
	struct CONSTANT_Float_info * e
	  = (struct CONSTANT_Float_info *)v_ptr;
	
	printf("+ Data: %f\n", e->data);
      }
      break;
    case CONSTANT_Long:
      {
	printf("+ Tag: Long\n");
        
	struct CONSTANT_Long_info * e
	  = (struct CONSTANT_Long_info *)v_ptr;
	
	printf("+ Data: %ld\n", e->data);
      }
      break;
    case CONSTANT_Double:
      {
	printf("+ Tag: Double\n");
	struct CONSTANT_Double_info * e
	  = (struct CONSTANT_Double_info *)v_ptr;
        printf("+ Data: %lf\n", e->data);
      }
      break;
    case CONSTANT_Class:
      {
	printf("+ Tag: Class\n");

	struct CONSTANT_Class_info * e
	  = (struct CONSTANT_Class_info *)v_ptr;
	printf("+ name_index: #%d\n++ <%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
      }
      break;
    case CONSTANT_String:
      {
	printf("+ Tag: String\n");

	struct CONSTANT_String_info * e
	  = (struct CONSTANT_String_info *)v_ptr;
	printf("+ string_index: #%d\n++ <%s>\n", e->string_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->string_index]))->data);
      }
      break;
    case CONSTANT_Fieldref:
      {
	printf("+ Tag: Fieldref\n");
	struct CONSTANT_Fieldref_info * e
	  = (struct CONSTANT_Fieldref_info *)v_ptr;

	short class_name_index = ((struct CONSTANT_Class_info *)(cp[e->class_index]))->name_index;
	printf("+ class_index: #%d\n++ <Class %s>\n", e->class_index,
	       ((struct CONSTANT_Utf8_info *)(cp[class_name_index]))->data);

	struct CONSTANT_NameAndType_info *nt_info = (struct CONSTANT_NameAndType_info *)(cp[e->name_and_type_index]);
	printf("+ name_and_type_index: #%d\n++ <Name:%s, Type:%s>\n+++ ", e->name_and_type_index,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->name_index]))->data,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
	parse_descriptor(((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
	
      }
      break;
    case CONSTANT_Methodref:
      {
	printf("+ Tag: Methodref\n");
	struct CONSTANT_Methodref_info * e
	  = (struct CONSTANT_Methodref_info *)v_ptr;

	short class_name_index = ((struct CONSTANT_Class_info *)(cp[e->class_index]))->name_index;
	printf("+ class_index: #%d\n++ <Class %s>\n", e->class_index,
	       ((struct CONSTANT_Utf8_info *)(cp[class_name_index]))->data);
	
	struct CONSTANT_NameAndType_info *nt_info = (struct CONSTANT_NameAndType_info *)(cp[e->name_and_type_index]);
	printf("+ name_and_type_index: #%d\n++ <Name:%s, Type:%s>\n+++ ", e->name_and_type_index,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->name_index]))->data,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
      }

      break;
    case CONSTANT_InterfaceMethodref:
      {
	printf("+ Tag: InterfaceMethodref\n");
	struct CONSTANT_InterfaceMethodref_info * e
	  = (struct CONSTANT_InterfaceMethodref_info *)v_ptr;

	short class_name_index = ((struct CONSTANT_Class_info *)(cp[e->class_index]))->name_index;
	printf("+ class_index: #%d\n++ <Class %s>\n", e->class_index,
	       ((struct CONSTANT_Utf8_info *)(cp[class_name_index]))->data);
	
	struct CONSTANT_NameAndType_info *nt_info = (struct CONSTANT_NameAndType_info *)(cp[e->name_and_type_index]);
	printf("+ name_and_type_index: #%d\n++ <Name:%s, Type:%s>\n+++ ", e->name_and_type_index,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->name_index]))->data,
	       ((struct CONSTANT_Utf8_info *)(cp[nt_info->descriptor_index]))->data);
      }
      break;
    case CONSTANT_NameAndType:
      {
	printf("+ Tag: NameAndType\n");

	struct CONSTANT_NameAndType_info * e
	  = (struct CONSTANT_NameAndType_info *)v_ptr;

	printf("+ name_index: #%d\n++ <%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
	
	printf("+ descriptor_index: #%d\n++ <%s>\n", e->descriptor_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->descriptor_index]))->data);
      }
      break;
    case CONSTANT_MethodHandle:
      {
	printf("+ Tag: MethodHandle\n");
	
	struct CONSTANT_MethodHandle_info * e
	  = (struct CONSTANT_MethodHandle_info *)v_ptr;
      }
      break;
    case CONSTANT_MethodType:
      {
	printf("+ Tag: MethodType\n");
	struct CONSTANT_MethodType_info * e
	  = (struct CONSTANT_MethodType_info *)v_ptr;
	printf("+ descriptor_index: #%d\n++ <%s>\n", e->descriptor_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->descriptor_index]))->data);
      }
      break;
    case CONSTANT_InvokeDynamic:
      {
	printf("+ Tag: InvokeDynamic\n");
	struct CONSTANT_InvokeDynamic_info * e
	  = (struct CONSTANT_InvokeDynamic_info *)v_ptr;
      }
      break;
    case CONSTANT_Module:
      {
	printf("+ Tag: Module\n");
	
	struct CONSTANT_Module_info * e
	  = (struct CONSTANT_Module_info *)v_ptr;
	printf("+ name_index: #%d\n++ <%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
      }
      break;
    case CONSTANT_Package:
      {
	printf("+ Tag: Package\n");
	struct CONSTANT_Package_info * e
	  = (struct CONSTANT_Package_info *)v_ptr;
	printf("+ name_index: #%d\n++ <%s>\n", e->name_index,
	       ((struct CONSTANT_Utf8_info *)(cp[e->name_index]))->data);
      }
      break;
    }
    printf("+++++++++++++++++\n");
  }
}

/**
 * help function to check access flags
 */
void access_flag_check(short accflg, int isMethod){
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
    if(isMethod)
      printf("ACC_BRIDGE\t");
    else
      printf("ACC_VOLATILE\t");
  }
  if(accflg & ACC_TRANSIENT){
    if(isMethod)
      printf("ACC_VARARGS\t");
    else
      printf("ACC_TRANSIENT\t");
    
  }
  if(accflg & ACC_SUPER){
    if(isMethod)
      printf("ACC_SYNCHRONIZED");
    else
      printf("ACC_SUPER\t");
  }
  if(accflg & ACC_NATIVE){
    printf("ACC_NATIVE\t");
  }
  if(accflg & ACC_INTERFACE){
    printf("ACC_INTERFACE\t");
  }
  if(accflg & ACC_ABSTRACT){
    printf("ACC_ABSTRACT\t");
  }
  if(accflg & ACC_STRICT){
    printf("ACC_STRICT\t");
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

void parse_attr(FILE *f, void**cp){
  short name_index = getshort(f);
  char* name = get_cp_utf8(cp, name_index);
  //printf("name index: #%d\n", name_index);
  int len = getint(f);
  printf("\n+++++ ATTR:%s +++++\n", name);
  printf("+ len:%d\n",len);

  if(!strcmp(name, "ConstantValue")){
    short value_index = getshort(f);
    printf("+ contant value index:%d\n", value_index);
    char* e = (char*)(cp[value_index]);
    
    switch(e[0]){
    case CONSTANT_Long:
      {
	printf("++ type: long\n");
	struct CONSTANT_Long_info* d = (struct CONSTANT_Long_info*)e;
	printf("++ value: %ld\n", d->data);
      }
    break;
    case CONSTANT_Float:
      {
	printf("++ type: float\n");
	struct CONSTANT_Float_info* d = (struct CONSTANT_Float_info*)e;
	printf("++ value: %f\n", d->data);
      }
    break;
    case CONSTANT_Double:
      {
	printf("++ type: double\n");
	struct CONSTANT_Double_info* d = (struct CONSTANT_Double_info*)e;
	printf("++ value: %lf\n", d->data);
      }
    break;
    case CONSTANT_Integer:
      {
	printf("++ type: int/short/char/byte/boolean\n");
	struct CONSTANT_Integer_info* d = (struct CONSTANT_Integer_info*)e;
	printf("++ value: %d\n", d->data);
      }
    break;
    case CONSTANT_String:
      {
	printf("++ type: String\n");
	struct CONSTANT_String_info* d = (struct CONSTANT_String_info*)e;
	printf("++ value: %s\n", get_cp_utf8(cp, d->string_index));
      }
    break;
    }
  } else if(!strcmp(name, "Code")){
    /**
     * Code Attribute:
     * u2 - max_stack
     * u2 - max_locals
     * u4 - code_length
     * u1 - code[code_length]
     * u2 - exception_table_length
     * u8 - exception_table
     * u2 - attributes_count
     * attribute_info attributes[attributes_count]
     */
    //char* code = getnbytes(f, len);
    short max_stack = getshort(f);
    short max_locals = getshort(f);
    printf("++ max_stack: %d\n", max_stack);
    printf("++ max_locals: %d\n", max_locals);
    int code_len = getint(f);
    printf("++ code_length: %d\n", code_len);
    char* code = getnbytes(f, code_len);
    printf("+++ Code +++\n\n");
    printbytes(code, code_len);
    printf("\n+++ End of Code +++\n\n");
    
    short ex_len = getshort(f);
    printf("++ exception_table_length: %d\n", ex_len);
    for(short i = 0; i < ex_len; i++){
      printf("+++ Exception %d\n\n", i+1);
      short start_pc = getshort(f);
      short end_pc = getshort(f);
      short handler_pc = getshort(f);
      short catch_type = getshort(f);
      printf("+++ start_pc: #%d\n", start_pc);
      printf("+++ end_pc: #%d\n", end_pc);
      printf("+++ handler_pc: #%d\n", handler_pc);
      printf("+++ catch_type: #%d\n", catch_type);
    }

    short att_len = getshort(f);
    printf("++ attribute_count: %d\n", att_len);
    if(att_len){
      printf("+++ Attributes \n\n");
      for(short i = 0; i < att_len; i ++){
	parse_attr(f, cp);
      }
      printf("+++ End of Attributes \n\n");  
    }
    
  } else if(!strcmp(name, "StackMapTable")){
    short num_of_entries = getshort(f);
    printf("+ number of entries: %d\n", num_of_entries);
    for(short i = 0; i < num_of_entries; i++){
      printf("++ENTRY %d\n", i);
      parse_stack_map_frame(f);
    }
  } else if(!strcmp(name, "Exceptions")){
    char* info = getnbytes(f, 10);
  } else if(!strcmp(name, "InnerClasses")){
    char* info = getnbytes(f, 6);
    short num = getshort(f);
    for(short i = 0; i < num; i++){
      char* classinfo = getnbytes(f, 8);
    }
  } else if(!strcmp(name, "EnclosingMethod")){
    char* info = getnbytes(f, 4);
  } else if(!strcmp(name, "Synthetic")){
    
  } else if(!strcmp(name, "Signature")){
    short info = getshort(f);
  } else if(!strcmp(name, "SourceFile")){
    short info = getshort(f);
  } else if(!strcmp(name, "SourceDebugExtension")){
    char* info = getnbytes(f, 1);
  } else if(!strcmp(name, "LineNumberTable")){
    
    short num = getshort(f);
    printf("+ line_number_table_length: %d\n", num);
    for(short i = 0; i < num; i++){
      printf("++ line_number_table_entry %d +++\n", i);
      short start_pc = getshort(f);
      short line_num = getshort(f);
      
      printf("+++ start_pc: %d\n", start_pc);
      printf("+++ line_number: %d\n", line_num);
    }
  } else{
    getnbytes(f, len);
  }


  
}

void parse_verification_type_info(FILE *fp){
  char tag = getachar(fp);
  
  switch (tag){
  case ITEM_Top:
    printf("++++ verification type: top\n");
    break;
  case ITEM_Integer:
    printf("++++ verification type: int\n");
    break;
  case ITEM_Float:
    printf("++++ verification type: float\n");
    break;
  case ITEM_Null:
    printf("++++ verification type: null\n");
    break;
  case ITEM_UninitializedThis:
    printf("++++ verification type: uninitializedThis\n");
    break;
  case ITEM_Object:
    {
      printf("++++ verification type: Object\n");
      short cp_index = getshort(fp);
    }
    break;
  case ITEM_Uninitialized:
    {
      short off = getshort(fp);
      printf("++++ verification type: uninitialized (offset:%d)\n", off);
    }
    break;
  case ITEM_Long:
    printf("++++ verification type: long\n");
    break;
  case ITEM_Double:
    printf("++++ verification type: double\n");
    break;
  }
}

void parse_stack_map_frame(FILE *fp){
  char tag = getachar(fp);
  if(tag < 0){
    fprintf(stderr, "error in parse_stack_map_frame\n");
  } else if (tag < 64){
    printf("+++ same_frame\n");
    printf("+++ offset_delta: %d\n", tag);
  } else if (tag < 128){
    printf("+++ same_locals_1_stack_item_frame\n");
    printf("+++ delta_offset: %d\n", tag - 64);
    parse_verification_type_info(fp);
  } else if (tag < 247){
    fprintf(stderr, "reserved value in parse_stack_map_frame\n");
  } else if (tag == 247){
    printf("+++ same_locals_1_stack_item_frame_extended\n");
    short offset = getshort(fp);
    printf("+++ offset_delta: %d\n", offset);
    parse_verification_type_info(fp);
  } else if (tag < 251){
    printf("+++ chop_frame\n");
    short offset = getshort(fp);
    printf("+++ offset_delta: %d\n", offset);
  } else if (tag == 251){
    printf("+++ same_frame_extended\n");
    short offset = getshort(fp);
    printf("+++ offset_delta: %d\n", offset);
  }  else if (tag < 255){
    printf("+++ append_frame\n");
    short offset = getshort(fp);
    printf("+++ offset_delta: %d\n", offset);
    for(short i = 0; i < tag-251; i++){
      parse_verification_type_info(fp);
    }
  }  else if (tag == 255){
    printf("+++ full_frame\n");
    short offset = getshort(fp);
    printf("+++ offset_delta: %d\n", offset);
    short num_of_locals = getshort(fp);
    printf("+++ locals count: %d\n", num_of_locals);
    for(short i = 0; i < num_of_locals; i ++){
      parse_verification_type_info(fp);
    }
    short num_of_items = getshort(fp);
    printf("+++ stack items count: %d\n", num_of_items);
    for(short i = 0; i < num_of_items; i ++){
      parse_verification_type_info(fp);
    }
  }
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
  
  printf("*******************\n");
  printf("*******************\n");
  printf("*  Constant Pool  *\n");
  printf("*******************\n");
  printf("*******************\n");
  
  short cp_count = getshort(fp);
  //printf("=====Constant Pool=====\n\t");
  printf("Size:%d\n", cp_count);

  void** cp = (void**) malloc(sizeof(void*) * cp_count);
  
  for(short i = 1; i < cp_count; i++){
    //get the tag
    char tag = getachar(fp);
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
	//char* bytes_int = getnbytes(fp, 4);
	int data = getint(fp);

	struct CONSTANT_Integer_info * e
	  = (struct CONSTANT_Integer_info *)malloc(sizeof(struct CONSTANT_Integer_info));
	e->tag = tag;
	e->data = data;
	cp[i] = (void*)e;
      }
      break;
    case CONSTANT_Float:
      {
	int int_data = getint(fp);
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
	char data1 = getachar(fp);
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
  access_flag_check(access_flag, 0);
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
  printf("*******************\n");
  printf("*******************\n");
  printf("*   F i e l d s   *\n");
  printf("*******************\n");
  printf("*******************\n");
  
  short fields_count = getshort(fp);
  printf("fields_count:\t\t%d\n", fields_count);
  for (short i = 0; i < fields_count; i ++){
    printf("-----field_info %d-----\n", i);
    short f_acc_flag = getshort(fp);
    printf("access_flag:\t\t0x%04X\n", f_acc_flag);
    if(f_acc_flag){
      printf("\t");
      access_flag_check(f_acc_flag, 0);
      printf("\n");
    }
    

    short f_name = getshort(fp);
    printf("name_index:\t\t#%d\n", f_name);
    printf("  <name: %s>\n", get_cp_utf8(cp, f_name));

    short f_descriptor = getshort(fp);
    printf("descriptor_index:\t#%d\n  ", f_descriptor);
    parse_descriptor(get_cp_utf8(cp, f_descriptor));

    short f_att_count = getshort(fp);
    printf("f_att_count:\t\t%d\n", f_att_count);
    if(f_att_count){
      for(short j = 0; j < f_att_count; j++){
	parse_attr(fp, cp);
      }
    }
    printf("\n");
  }

  /* methods */
  
  printf("*******************\n");
  printf("*******************\n");
  printf("*  M e t h o d s  *\n");
  printf("*******************\n");
  printf("*******************\n");
  
  short methods_count = getshort(fp);
  printf("methods_count:\t\t%d\n", methods_count);
  for (short i = 0; i < methods_count; i ++){
    printf("========================\n");
    printf("-----method_info %d-----\n", i);
    printf("========================\n");
    short m_acc_flag = getshort(fp);
    printf("access_flag:\t\t0x%04X\n", m_acc_flag);
    if(m_acc_flag){
      printf("\t");
      access_flag_check(m_acc_flag, 1);
      printf("\n");
    }
    short f_name = getshort(fp);
    printf("name_index:\t\t#%d\n", f_name);
    printf("     <name: %s>\n", get_cp_utf8(cp, f_name));
    
    short f_descriptor = getshort(fp);
    printf("descriptor_index:\t#%d\n", f_descriptor);
    printf("     <descriptor: %s>\n", get_cp_utf8(cp, f_descriptor));
    
    //parse_descriptor(get_cp_utf8(cp, f_descriptor));

    short f_att_count = getshort(fp);
    printf("f_att_count:\t\t%d\n", f_att_count);
    if(f_att_count){
      for(short j = 0; j < f_att_count; j++){
	parse_attr(fp, cp);
      }
    }
    printf("\n");
  }

  printf("*******************\n");
  printf("*******************\n");
  printf("*    Attributes   *\n");
  printf("*******************\n");
  printf("*******************\n");

  short att_count = getshort(fp);
  printf("Attributes Count: %d", att_count);
  if(att_count){
    for(short j = 0; j < att_count; j++){
      parse_attr(fp, cp);
    }
  }
  printf("\n");

  printf("\n\n====End of Class File====\n");
  
}
