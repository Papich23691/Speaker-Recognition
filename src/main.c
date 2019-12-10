#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"

int main(void)
{
    /* Recording */
    paData data;
    record(&data);
    playback(data);

    /* Pre emphasis and framing */
    float *arr = (float *)malloc(sizeof(float) * data.maxFrameIndex);
    data_pre_emphasis(data, &arr);
    float complex frames[FRAMES][FRAME_SIZE];
    memcpy(frames, framing(arr), sizeof(frames));
    free(arr);

    /* Hamming Window */
    window(&frames);

    /* Fast Fourier Transform */
    float complex *bins = (float complex *)malloc(sizeof(float complex) * FRAME_SIZE);
    for (int i=0;i<FRAMES;i++)
    {
        fft(frames[i],&bins,FRAME_SIZE);
        /* Power Spectrum */
        power_spectrum(&bins);
    }
    free(bins);
    return 0;
}
