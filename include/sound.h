#include <stdio.h>
#include <complex.h> 
#include "record.h"

#define PRE_EMPHASIS 0.97
#define FRAME_SIZE 1024
#define STRIDE 512
#define FRAMES (int)(((SECONDS * SAMPLE_RATE) - FRAME_SIZE ) / (FRAME_SIZE - STRIDE)) + 1
#define TO_HZ(bin) bin * (int)(SAMPLE_RATE/FRAME_SIZE)

void data_pre_emphasis(paData data, float **arr);
float complex **framing(float *recording);
void window(float complex (*frames)[FRAMES][FRAME_SIZE]);
void fft(float complex *samples,float complex **out,int N);
void power_spectrum(float complex **bins);