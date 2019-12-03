#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"

int main(void)
{
    paData data;
    record(&data);
    playback(data);
    float *arr = (float *)malloc(sizeof(float) * data.maxFrameIndex);
    data_pre_emphasis(data, &arr);
    float frames[FRAMES][(int)(FRAME_SIZE * SAMPLE_RATE)];
    memcpy(frames, framing(arr),  sizeof(frames));
    free(arr);
    return 0;
}
