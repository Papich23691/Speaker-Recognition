#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

int main(void)
{
    float speaker[CODEWORDS][MEL_COEFFICIENTS + 1];
    model(speaker);
    return 0;
}
