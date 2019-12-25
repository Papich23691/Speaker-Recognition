#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

int main(void)
{
    float speaker1[CODEWORDS][MEL_COEFFICIENTS + 1];
    float speaker2[CODEWORDS][MEL_COEFFICIENTS + 1];
    model(speaker1);
    model(speaker2);
    printf("Score = %f\n",compare(speaker1,speaker2));
    save(speaker1,"speaker1.data");
    return 0;
}
