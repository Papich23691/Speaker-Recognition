#include <stdio.h>
#include <stdlib.h>
#include "sound.h"

void data_pre_emphasis(paData data, SAMPLE **arr){
    (*arr)[0] = data.recordedSamples[0];
    for (int i=2;i<(data.maxFrameIndex * 2)-1;i+=2)
    {
        (*arr)[i/2] = data.recordedSamples[i] - PRE_EMPHASIS * (*arr)[(i/2)-1];
    }
}