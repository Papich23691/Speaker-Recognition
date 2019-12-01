#include <stdio.h>
#include <stdlib.h>
#include "record.h"

int main(void)
{
    paData data;
    record(5, &data);
    playback(data);
    return 0;
}
