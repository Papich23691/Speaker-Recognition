#include <stdio.h>
#include <stdlib.h>
#include "record.h"

int main(void)
{
   SAMPLE *recording;
   record(5,&recording);
   for (int i=0;i<SAMPLE_RATE;i++){
       printf("%d = %f\n",i,recording[i]);
   }
   return 0;
}
