#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mfcc.h"

int main(void)
{
    float *f_vector = (float *)malloc(sizeof(float) * MEL_COEFFICIENTS);
    mfcc(&f_vector);
    return 0;
}