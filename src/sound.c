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

void fft(float complex *samples,float complex **out,int N)
{
    if (N > 1)
    {
        int i;
        float complex *even = (float complex *)malloc(sizeof(float complex) * (int)N/2);
        float complex *odd = (float complex *)malloc(sizeof(float complex) * (int)N/2);
        for (i=0;i<(int)N/2;i++)
        {
            even[i] = samples[i*2];
            odd[i] = samples[i*2 + 1];
        }

        fft(even,&even,(int)N/2);
        fft(odd,&odd,(int)N/2);

        for (i=0;i<(int)N/2;i++)
        {
            float complex w = cexp(-I*2*M_PI*i/N) * odd[i];
            (*out)[i] = even[i] + w;
            (*out)[i + N/2] = even[i] - w;
        }
        free(even);
        free(odd);
    }
}
