#include "sound.h"

#define FILTERS 32
#define MEL_COEFFICIENTS 36
#define MEL_NO_DELTAS 12
#define DELTA 0
#define D_DELTA 1
#define N 2
#define MEL(f) (2595 * log10(1 + (f / 700)))
#define I_MEL(m) (700 * (pow(10, (m / 2595)) - 1))

int *filter_points();
void filter_banks(float complex *bins, float banks[FILTERS]);
void dct(float banks[FILTERS], float mfcc[FILTERS]);
void mfcc(float f_vector[FRAMES][MEL_COEFFICIENTS]);
void delta(float f_vector[FRAMES][MEL_COEFFICIENTS], int state);