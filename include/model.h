#include <stdio.h>
#include <stdlib.h>
#include "mfcc.h"

#define CODEWORDS 32

/* float cos_sim(float *v1, float *v2, int dimension); */
float dot(float *v1, float *v2, int dimension);
float e_dis(float *v1, float *v2, int dimension);
void add(float *v1, float *v2, int dimension);
void v_div(float *v1, float d, int dimension);
void random_v(float codebook[CODEWORDS][MEL_COEFFICIENTS + 1], float f_vector[FRAMES][MEL_COEFFICIENTS]);
void model(float codebook[CODEWORDS][MEL_COEFFICIENTS + 1]);
float compare(float speaker1[CODEWORDS][MEL_COEFFICIENTS + 1], float speaker2[CODEWORDS][MEL_COEFFICIENTS + 1]);
void save(float speaker[CODEWORDS][MEL_COEFFICIENTS + 1], char *name);
void load(float speaker[CODEWORDS][MEL_COEFFICIENTS + 1], char *name);