#include <stdio.h>
#include <stdlib.h>
#include "sound.h"

int main(void)
{
    paData data;
    record(5, &data);
    //playback(data);
    for (int i=0;i<(data.maxFrameIndex * 2)-1;i+=2)
    {
        printf("*************** %f - %f\n",data.recordedSamples[i],data.recordedSamples[i+1]);
    }
    SAMPLE *arr = (SAMPLE *)malloc(sizeof(SAMPLE) * data.maxFrameIndex);
    data_pre_emphasis(data,&arr);
    for (int i=0;i<data.maxFrameIndex;i++)
    {
        printf("%f\n",arr[i]);
    }
    free(arr);
    return 0;
}
