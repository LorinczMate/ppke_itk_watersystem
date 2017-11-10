#include "utility.h"

/**
   Előfelétel: Az *array tömbben legyen legalább length+1 szabad hely
*/
void arrayShiftRight(char length, char *array, char newElement){
   for (unsigned char i=length; i>0; --i){
      array[i]=array[i-1];
   }
   array[0]=newElement;
}
