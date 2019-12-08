#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"

#define TEST 8

int main(void)
{
    paData data;
    record(&data);
    playback(data);
    float *arr = (float *)malloc(sizeof(float) * data.maxFrameIndex);
    data_pre_emphasis(data, &arr);
    float frames[FRAMES][(int)(FRAME_SIZE * SAMPLE_RATE)];
    memcpy(frames, framing(arr), sizeof(frames));
    free(arr);
    window(&frames);
    double complex *bins = (float complex *)malloc(sizeof(float complex) * (int)(FRAME_SIZE * SAMPLE_RATE));
    fft(frames[100],&bins,(int)(FRAME_SIZE * SAMPLE_RATE));
    free(bins);
    return 0;
}
