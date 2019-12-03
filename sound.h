#include <stdio.h>
#include "record.h"

#define PRE_EMPHASIS 0.97
#define FRAME_SIZE 0.025
#define STRIDE 0.01
#define FRAMES (int)(((SECONDS * SAMPLE_RATE) - (FRAME_SIZE * SAMPLE_RATE)) / ((FRAME_SIZE - STRIDE) * SAMPLE_RATE) + 1)

void data_pre_emphasis(paData data, float **arr);
float **framing(float *recording);