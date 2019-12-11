#include "sound.h"

#define FILTERS 40
#define MEL(f) 2595 * log10(1 + (f/700))
#define I_MEL(m) 700 * (pow(10,(m/2595)) - 1)

int *filter_points();
void filter_banks(float complex *bins,float *banks[FILTERS]);
void dct(float banks[FILTERS],float *mfcc[FILTERS]);