#include "classdump.h"

Classdump::Classdump(char* fileName){
  std::cout << "opening file " << fileName << std::endl;
}

void Classdump::parsecp(void* start, int size){
  void * ptr = start;
  for (int i = 1; i < size; i ++){
    byte tag = &(byte *) ptr;
    ptr = (void*)((char* ptr) + 1);
    switch (tag) {
    case 1:
      //utf8
      short length = &(short*)ptr;
      ptr = (void*)((short* ptr) + 1);
      char* data = strndup((char*) ptr, length);
      std::cout << "utf8 " << data << std::endl;
      ptr = (void*)((char* ptr) + 1ength);
    }
  }
}
