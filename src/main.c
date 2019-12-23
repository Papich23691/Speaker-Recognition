#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

int main(void)
{
    float speaker[CODEWORDS][MEL_COEFFICIENTS + 1];
    model(speaker);
    save(speaker,"speaker.data");
    return 0;
}
