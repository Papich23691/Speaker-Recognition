#include <stdio.h>
#include <math.h>
#include "sound.h"

void data_pre_emphasis(paData data, float **arr)
{
    (*arr)[0] = data.recordedSamples[0];
    for (int i = 2; i < (data.maxFrameIndex * 2) - 1; i += 2)
        (*arr)[i / 2] = data.recordedSamples[i] - PRE_EMPHASIS * data.recordedSamples[i - 2];
}

float **framing(float *recording)
{
    int i, j, k = 0;
    static float frames[FRAMES][(int)(FRAME_SIZE * SAMPLE_RATE)];
    for (i = 0; i < (SECONDS * SAMPLE_RATE) - (FRAME_SIZE * SAMPLE_RATE); i += (FRAME_SIZE - STRIDE) * SAMPLE_RATE)
    {
        for (j = 0; j < (FRAME_SIZE * SAMPLE_RATE); j++)
            frames[k][j] = recording[i + j];
        ++k;
    }
    return frames;
}

void window(float (*frames)[FRAMES][(int)(FRAME_SIZE * SAMPLE_RATE)])
{
    for (int i = 0; i < FRAMES; i++)
    {
        for (int j = 0; j < (FRAME_SIZE * SAMPLE_RATE); j++)
            (*frames)[i][j] *= 0.54 - (0.46 * cos((2 * M_PI * j) / ((FRAME_SIZE * SAMPLE_RATE) - 1)));
    }
}