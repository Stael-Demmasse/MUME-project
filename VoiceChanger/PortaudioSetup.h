#ifndef PORTAUDIOSETUP_H
#define PORTAUDIOSETUP_H

#include <portaudio.h>
#include <QString>

#include "Structs.h"

void initializePa(PaError* err);
void terminatePa(PaError* err);
void createStream(PaStream** stream, callbackData* userData, PaError* err);
void closeStream(PaStream* stream, PaError* err);
void startStream(PaStream* stream, PaError* err);
void stopStream(PaStream* stream, PaError* err);
void handleError(int err);
void errorCodeOutput(QString stage, PaError* err);

static int callback (
    const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
    void* userData);

void getDeviceInfo();
void getDefaultInOut();

#endif
