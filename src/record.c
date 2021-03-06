#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "record.h"

/* Recording function used by portaudio engine per frame to save data */
int recordCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData)
{
    paData *data = (paData *)userData;
    const float *rptr = (const float *)inputBuffer;
    float *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void)outputBuffer; /* Prevent unused variable warnings. */
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    if (framesLeft < framesPerBuffer)
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if (inputBuffer == NULL)
    {
        for (i = 0; i < framesToCalc; i++)
        {
            *wptr++ = SAMPLE_SILENCE; /* left */
            if (NUM_CHANNELS == 2)
                *wptr++ = SAMPLE_SILENCE; /* right */
        }
    }
    else
    {
        for (i = 0; i < framesToCalc; i++)
        {
            *wptr++ = *rptr++; /* left */
            if (NUM_CHANNELS == 2)
                *wptr++ = *rptr++; /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

/* 
 * Record audio for selected defined seconds 
 * and save into paData type.
 */
int record(paData *recording)
{
    PaStreamParameters inputParameters;
    PaStream *stream;
    PaError err = paNoError;
    static paData data;
    int i;
    int totalFrames;
    int numSamples;
    int numBytes;

    data.maxFrameIndex = totalFrames = SECONDS * SAMPLE_RATE; /* Record for a few SECONDS. */
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(float);
    data.recordedSamples = (float *)malloc(numBytes); /* From now on, recordedSamples is initialised. */
    if (data.recordedSamples == NULL)
    {
        printf("Could not allocate record array.\n");
        return 1;
    }

    for (i = 0; i < numSamples; i++)
        data.recordedSamples[i] = 0;

    err = Pa_Initialize();
    if (err != paNoError)
        return 1;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice)
    {
        fprintf(stderr, "Error: No default input device.\n");
        return 1;
    }
    inputParameters.channelCount = 2; /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio */
    err = Pa_OpenStream(
        &stream,
        &inputParameters,
        NULL, /* &outputParameters, */
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,
        recordCallback,
        &data);
    if (err != paNoError)
        return 1;

    err = Pa_StartStream(stream);
    if (err != paNoError)
        return 1;

    while ((err = Pa_IsStreamActive(stream)) == 1)
        ;
    if (err < 0)
        return 1;

    err = Pa_CloseStream(stream);
    if (err != paNoError)
        return 1;

    *recording = data;
    return 0;
}

/* Playback function used by portaudio engine per frame to play data */
int playCallback(const void *inputBuffer, void *outputBuffer,
                 unsigned long framesPerBuffer,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 PaStreamCallbackFlags statusFlags,
                 void *userData)
{
    paData *data = (paData *)userData;
    float *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    float *wptr = (float *)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void)inputBuffer; /* Prevent unused variable warnings. */
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    if (framesLeft < framesPerBuffer)
    {
        /* final buffer... */
        for (i = 0; i < framesLeft; i++)
        {
            *wptr++ = *rptr++; /* left */
            if (NUM_CHANNELS == 2)
                *wptr++ = *rptr++; /* right */
        }
        for (; i < framesPerBuffer; i++)
        {
            *wptr++ = 0; /* left */
            if (NUM_CHANNELS == 2)
                *wptr++ = 0; /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    }
    else
    {
        for (i = 0; i < framesPerBuffer; i++)
        {
            *wptr++ = *rptr++; /* left */
            if (NUM_CHANNELS == 2)
                *wptr++ = *rptr++; /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

/* Play audio using saved paData audio */
int playback(paData data)
{
    PaError err = paNoError;
    PaStream *stream;
    PaStreamParameters outputParameters;
    data.frameIndex = 0;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice)
    {
        fprintf(stderr, "Error: No default output device.\n");
        return 1;
    }
    outputParameters.channelCount = 2; /* stereo output */
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff, /* we won't output out of range samples so don't bother clipping them */
        playCallback,
        &data);
    if (err != paNoError)
        return 1;

    if (stream)
    {
        err = Pa_StartStream(stream);
        if (err != paNoError)
            return 1;

        while ((err = Pa_IsStreamActive(stream)) == 1)
            ;
        if (err < 0)
            return 1;

        err = Pa_CloseStream(stream);
        if (err != paNoError)
            return 1;
    }
    return 0;
}
