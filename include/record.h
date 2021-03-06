#include "portaudio.h"

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_CHANNELS (2)
#define SECONDS 5

#define PA_SAMPLE_TYPE paFloat32
#define SAMPLE_SILENCE (0.0f)
#define PRINTF_S_FORMAT "%.8f"

typedef struct
{
    int frameIndex;
    int maxFrameIndex;
    float *recordedSamples;
} paData;

int recordCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData);

int record(paData *recording);

int playCallback(const void *inputBuffer, void *outputBuffer,
                 unsigned long framesPerBuffer,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 PaStreamCallbackFlags statusFlags,
                 void *userData);

int playback(paData data);
