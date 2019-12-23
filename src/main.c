#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

int main(void)
{
    float f_vector1[FRAMES][MEL_COEFFICIENTS];
    float speaker1[CODEWORDS][MEL_COEFFICIENTS + 1];
    float f_vector2[FRAMES][MEL_COEFFICIENTS];
    float speaker2[CODEWORDS][MEL_COEFFICIENTS + 1];
    
    mfcc(f_vector1);
    codebook(speaker1,f_vector2);
    mfcc(f_vector2);
    codebook(speaker2,f_vector2);

    printf("compare- %f\n",compare(speaker1,speaker2));


    return 0;
}
