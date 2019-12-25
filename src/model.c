#include "model.h"

/*float cos_sim(float *v1, float *v2, int dimension)
{
    return dot(v1, v2, dimension) / (sqrt(dot(v1, v1, dimension)) * sqrt(dot(v2, v2, dimension)));
}*/

/* Dot Product */
float dot(float *v1, float *v2, int dimension)
{
    float d = 0;
    int i;
    for (int i = 0; i < dimension; i++)
        d += v1[i] * v2[i];
    return d;
}

/* Euclidean Distance */
float e_dis(float *v1, float *v2, int dimension)
{
    float v3[dimension];
    for (int i = 0; i < dimension; i++)
        v3[i] = v2[i] - v1[i];
    return dot(v3, v3, dimension);
}

/* Adding 2 vectors */
void add(float *v1, float *v2, int dimension)
{
    int i;
    for (i = 0; i < dimension; i++)
    {
        v1[i] += v2[i];
    }
}

/* Divides vector */
void v_div(float *v1, float d, int dimension)
{
    if (d)
    {
        int i;
        for (i = 0; i < dimension; i++)
            v1[i] /= d;
    }
}

/* Picks random centroids */
void random_v(float codebook[CODEWORDS][MEL_COEFFICIENTS + 1], float f_vector[FRAMES][MEL_COEFFICIENTS])
{
    srand(time(NULL));
    int i, j, rnd = 0;
    int random[FRAMES];
    for (i = 0; i < CODEWORDS; i++)
    {
        do
            rnd = rand() % FRAMES;
        while (random[rnd]);
        for (j = 0; j < MEL_COEFFICIENTS; j++)
            codebook[i][j] = f_vector[rnd][j];
        ++random[rnd];
    }
}

/* GLA Vector Quantization */
void model(float codebook[CODEWORDS][MEL_COEFFICIENTS + 1])
{
    int i, j, min_c;
    int clusters[FRAMES];
    float distortion = 0, pre = 1, min, distance;
    static float codebook2[CODEWORDS][MEL_COEFFICIENTS + 1];
    float f_vector[FRAMES][MEL_COEFFICIENTS];
    mfcc(f_vector);
    random_v(codebook2, f_vector);

    while (abs(pre - distortion) > 0)
    {
        pre = distortion;
        memcpy(codebook, codebook2, sizeof(codebook2));
        distortion = 0;

        /* Clustering */
        for (i = 0; i < FRAMES; i++)
        {
            min_c = 0;
            min = e_dis(f_vector[i], codebook2[0], MEL_COEFFICIENTS);
            for (j = 1; j < CODEWORDS; j++)
            {
                distance = (float)sqrt(e_dis(f_vector[i], codebook2[j], MEL_COEFFICIENTS));
                if (distance < min)
                {
                    min_c = j;
                    min = distance;
                }
            }
            clusters[i] = min_c;
        }
        memset(codebook2, 0, sizeof(codebook2));

        /* Moving centroids */
        for (i = 0; i < FRAMES; i++)
        {
            add(codebook2[clusters[i]], f_vector[i], MEL_COEFFICIENTS);
            ++codebook2[clusters[i]][MEL_COEFFICIENTS];
        }

        for (i = 0; i < CODEWORDS; i++)
            v_div(codebook2[i], codebook2[i][MEL_COEFFICIENTS], MEL_COEFFICIENTS);

        /* distortion */
        for (i = 0; i < FRAMES; i++)
            distortion += (float)e_dis(codebook2[clusters[i]], f_vector[i], MEL_COEFFICIENTS) / (FRAMES);
    }
}

/* Compared by using average distortion */
float compare(float speaker1[CODEWORDS][MEL_COEFFICIENTS + 1], float speaker2[CODEWORDS][MEL_COEFFICIENTS + 1])
{
    float sum = 0, dis = 0, min_dis = 0;
    int i, j;
    for (i = 0; i < CODEWORDS; i++)
    {
        min_dis = e_dis(speaker1[i], speaker2[0], MEL_COEFFICIENTS);
        for (j = 1; j < CODEWORDS; j++)
        {
            dis = e_dis(speaker1[i], speaker2[j], MEL_COEFFICIENTS);
            min_dis = dis < min_dis ? dis : min_dis;
        }
        sum += min_dis;
    }
    return 1 / (1 + (sum / CODEWORDS));
}

void save(float speaker[CODEWORDS][MEL_COEFFICIENTS + 1], char *name)
{
    FILE *f = fopen(name, "wb");
    fwrite(speaker, sizeof(float), CODEWORDS * (MEL_COEFFICIENTS + 1), f);
    fclose(f);
}

void load(float (*speaker)[CODEWORDS][MEL_COEFFICIENTS + 1], char *name)
{
    FILE *f = fopen(name, "rb");
    fread(speaker, sizeof(float), CODEWORDS * (MEL_COEFFICIENTS + 1), f);
    fclose(f);
}