using namespace std;
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <cstdint>
//#include <stdio>
class Classdump
{
public:
  struct cpentry{
    int tag;
    char* data;
  };
  
    int32_t magic_number;                    /* 4 bytes that should be 0xCAFEBABE or it compiled wrong       */
    uint16_t majorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    uint16_t minorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    uint16_t constant_pool_count;            /* Number of items in constant pool plus one                    */
    cpentry *constant_pool;                  /* Strings containing all constants, interfaces, and fields     */
    short access_flag;                       /* Mask determining the access properties of this class         */
    short this_class;                        /* Index of self CONSTANT_Class_info in constant pool           */
    short super_class;                       /* Index of superclass CONSTANT_Class_info or 0 if Object class */
    short interfaces_count;                  /* The number of direct superinterfaces to this class           */
    char **interfaces;                       /* Each element is an index into constant pool for interfaces   */
    short fields_count;                      /* The number of field_info structures in the field table       */
    char **fields;                           /* Each element of the table is a field_info structure          */
    short methods_count;                     /* The number of method_info structures in the method table     */
    char **methods;                          /* Each element is a method_info class for all declared methods */
    short attributes_count;                  /* The number of attributes in the attributes table             */
    char **attributes;                       /* Additional descriptive information                           */
    bool bigEndian;                          /* True if machine is bigEndian, false otherwise                */

    Classdump(char *fileName);

    void parsecp(void* start, int size);
};

int main(int argc, char ** argv){
  Classdump cf(argv[1]);
  int startingPoint = atoi(argv[2]);
  std::cout << "starting from " << startingPoint << std::endl;
}
