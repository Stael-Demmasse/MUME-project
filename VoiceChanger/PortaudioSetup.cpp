#include <iostream>
#include <stdlib.h>
#include <fftw3.h>

#include "PortaudioSetup.h"
#include "Constants.h"
#include "FilterRobot.h"
#include "FilterPitchShift.h"
#include "FilterEcho.h"


void initializePa(PaError* err) {
    *err = Pa_Initialize();
    errorCodeOutput("Initialize", err);
    handleError(*err);
}

void terminatePa(PaError* err) {
    *err = Pa_Terminate();
    errorCodeOutput("Terminate", err);
    handleError(*err);
}

void createStream(PaStream** stream, callbackData* userData, PaError* err) {
    *err = Pa_OpenDefaultStream(
        stream,
        CHANNELS,   //input
        CHANNELS,   //output
        paFloat32,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        callback,
        userData);
    errorCodeOutput("Create", err);
    handleError(*err);
    if (*err != paNoError) {
        terminatePa(err);
        return;
    }
}

void closeStream(PaStream* stream, PaError* err) {
    *err = Pa_CloseStream(stream);
    errorCodeOutput("Close", err);
    handleError(*err);
}

void startStream(PaStream* stream, PaError* err) {
    *err = Pa_StartStream(stream);
    errorCodeOutput("Start", err);
    handleError(*err);
    if (*err != paNoError) {
        return;
    }
}

void stopStream(PaStream* stream, PaError* err) {
    *err = Pa_StopStream(stream);
    errorCodeOutput("Stop", err);
    handleError(*err);
}

/*
callback function used by portaudio on every new input
*/
static int callback (
    const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
    void* userData)
{
    float* in = (float*) inputBuffer;
    float* out = (float*) outputBuffer;
    callbackData* data = (callbackData*) userData;

    // applies a pitch shift to the audio output
    if (data->aFilter->pitchShift && data->aFilter->shiftFactor != 1) {
        if (!data->firstLoop) {
            pitchShiftEffect(in, out, data, framesPerBuffer);
        }
    }
    // needed because the other filters work with the output array as input
    else {
        for (int i = 0; i < framesPerBuffer; ++i) {
            out[i] = in [i];
        }
    }

    // applies an echo to the audio output
    if (data->aFilter->echo) {
        echoEffect(out, out, data->echoData, framesPerBuffer);
    }

    // applies a robotic sound to the audio output
    if (data->aFilter->robot) {
        robotVoice(out, data->carrierSignal, framesPerBuffer);
    }

    if (data->firstLoop) {
        data->firstLoop = false;
    }

    return 0;
}

/*
Exits the program if there is an error with portaudio
*/
void handleError(PaError err) {
    if (err != paNoError) {
        std::cerr << "\n\nPortAudio error: " << Pa_GetErrorText(err) << "\n\n" << std::endl;
        exit(1);
    }
}

/*
Outputs the error code message and the code itself
*/
void errorCodeOutput(QString stage, PaError* err) {
    std::cout << std::endl <<
        stage.toStdString() << " code: " << Pa_GetErrorText(*err) << " (" << *err << ")" <<
        std::endl;
}

/*
outputs the devices connected to the machine
*/
void getDeviceInfo() {
    int numDevices = Pa_GetDeviceCount();
    std::cout << "Number of devices: " << numDevices << "\n" << std::endl;
    if (numDevices < 0) {
        std::cout << "Error getting device count.\n" << std::endl;
        exit(1);
    }

    const PaDeviceInfo* deviceInfo;
    for (int i = 0; i < numDevices; ++i) {
        deviceInfo = Pa_GetDeviceInfo(i);
        std::cout << "Device " << i << ":" << std::endl;
        std::cout << "name: " << deviceInfo->name << std::endl;
        std::cout << "maxInputChannels: " << deviceInfo->maxInputChannels << std::endl;
        std::cout << "maxOutputChannels: " << deviceInfo->maxOutputChannels << std::endl;
        std::cout << "defaultSampleRate: " << deviceInfo->defaultSampleRate << "\n" << std::endl;
    }
}

void getDefaultInOut() {
    std::cout << "Default input device: " << Pa_GetDefaultInputDevice()  << std::endl
              << "default output device: " << Pa_GetDefaultOutputDevice() << std::endl;
}
