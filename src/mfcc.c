#include "mfcc.h"

int *filter_points()
{
    int i;
    float space = MEL(SAMPLE_RATE / 2) / (FILTERS + 1);
    static int filters[FILTERS + 2];
    filters[FILTERS + 1] = (FRAME_SIZE / 2) - 1;

    for (i = 0; i < FILTERS + 1; i++)
        filters[i] = TO_BIN(I_MEL(space * i));

    return filters;
}

void filter_banks(float complex *bins, float *banks[FILTERS])
{
    int i, j;
    int filters[FILTERS + 2];
    memcpy(filters, filter_points(), sizeof(filters));
    float filter, frequency, sum;
    for (i = 0; i < FILTERS; i++)
    {
        sum = 0;
        /* First half */
        for (int j = filters[i]; j < filters[i + 1]; j++)
        {
            float filter = (((float)j - (float)filters[i]) / ((float)filters[i + 1] - (float)filters[i]));
            float frequency = (float)creal(bins[j]);
            sum += filter * frequency;
        }

        /*Second half*/
        for (int j = filters[i + 1]; j < filters[i + 2]; j++)
        {
            float filter = (((float)filters[i + 2] - (float)j) / ((float)filters[i + 2] - (float)filters[i + 1]));
            float frequency = creal(bins[j]);
            sum += filter * frequency;
        }
        (*banks)[i] = sum;
    }
}

void dct(float banks[FILTERS], float *mfcc[FILTERS])
{
    int i, j;
    float sum;
    for (i = 0; i < FILTERS; i++)
    {
        sum = 0;
        for (j = 0; j < FILTERS; j++)
            sum += banks[j] * cos((2 * j + 1) * i * M_PI / (FILTERS * 2));

        (*mfcc)[i] = 2 * sum;
    }
}

void mfcc(float *f_vector[MEL_COEFFICIENTS])
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
    float *filters = (float *)malloc(sizeof(float) * FILTERS);
    float *mc = (float *)malloc(sizeof(float) * FILTERS);
    for (int i = 0; i < FRAMES; i++)
    {
        fft(frames[i], &bins, FRAME_SIZE);

        /* Power Spectrum */
        power_spectrum(&bins);

        /* Passing through mel filter banks */
        filter_banks(bins, &filters);

        /* Getting MFCC coefficients */
        dct(filters, &mc);
    }
    free(mc);
    free(filters);
    free(bins);
}