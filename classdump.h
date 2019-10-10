#include <string.h>
#include <stdio.h>
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
	      ACC_NATIVE = 0x0100,
	      ACC_INTERFACE = 0x0200,
	      ACC_ABSTRACT = 0x0400,
	      ACC_STRICT = 0x0800,
	      ACC_SYNTHETIC = 0x1000,
	      ACC_ANNOTATION = 0x2000,
	      ACC_ENUM = 0x4000,
	      ACC_MODULE = 0x8000
};

enum verification_type_info_tag{
				ITEM_Top = 0,
				ITEM_Integer = 1,
				ITEM_Float = 2,
				ITEM_Null = 5,
				ITEM_UninitializedThis = 6,
				ITEM_Object = 7,
				ITEM_Uninitialized = 8,
				ITEM_Long = 4,
				ITEM_Double = 3
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

char getachar(FILE* f);
short getshort(FILE* f);
int getint(FILE* f);
char* getnbytes(FILE* f, int n);
void printbytes(char* s, int len);
char* get_cp_utf8(void** cp, short index);
void parse_descriptor(char* str);
void print_cp(void** cp, short len);
void access_flag_check(short accflg, int isMethod);
void parse_attr(FILE *f, void **cp);
void parse_verification_type_info(FILE *fp);
void parse_stack_map_frame(FILE *fp);
