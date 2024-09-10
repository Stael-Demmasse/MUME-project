#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "Data.h"
#include "Constants.h"
#include "FilterRobot.h"

/*
Allocates disk space for a callbackData instance and initializes them if needed
*/
void initializeUserData(callbackData** userData) {
    *userData = (callbackData*)malloc(sizeof(callbackData));

    // initialize varibales needed for the pitch shift
    (*userData)->leftInBuffer = (double*)malloc(sizeof(double) * FRAMES_PER_BUFFER);
    (*userData)->inputWindow = (double*)malloc(sizeof(double) * FRAMES_PER_WINDOW);
    (*userData)->complexData = fftw_alloc_complex(FRAMES_PER_WINDOW/2+1);
    (*userData)->outputWindow = (double*)malloc(sizeof(double) * FRAMES_PER_WINDOW);
    (*userData)->leftOutBuffer = (double*)malloc(sizeof(double) * FRAMES_PER_BUFFER);
    zeroBuffer((*userData)->leftOutBuffer);

    // creates the Hann function which is needed for the windowing
    (*userData)->hannWindow = (double*)malloc(sizeof(double) * FRAMES_PER_WINDOW);
    createHannWindow((*userData)->hannWindow);

    // carrierSignal holds information needed for the robot filter
    (*userData)->carrierSignal = (carrierSignalInfos*)malloc(sizeof(carrierSignalInfos));
    (*userData)->carrierSignal->maxBuffers = ROBOT_VOICE_MAX_BUFFERS;
    (*userData)->carrierSignal->currentBuffer = 0;
    (*userData)->carrierSignal->signal = (double*)malloc(sizeof(double) * FRAMES_PER_BUFFER * ROBOT_VOICE_MAX_BUFFERS);
    createCarrierSignal((*userData)->carrierSignal);

    // echoData holds information needed for the echo filter
    (*userData)->echoData = (echoDataInfos*)malloc(sizeof(echoDataInfos));
    (*userData)->echoData->bufferSize = static_cast<int>(SAMPLE_RATE * ECHO_DELAY);
    (*userData)->echoData->buffer = (float*)malloc((*userData)->echoData->bufferSize * sizeof(float));
    std::fill_n((*userData)->echoData->buffer, (*userData)->echoData->bufferSize, 0.0f);  // Initialize buffer with zeros
    (*userData)->echoData->writeIndex = 0;

    // creates the Fourier plan and inverse Fourier plan
    (*userData)->plan = fftw_plan_dft_r2c_1d(
        FRAMES_PER_WINDOW, (*userData)->inputWindow, (*userData)->complexData, FFTW_ESTIMATE);
    (*userData)->iPlan = fftw_plan_dft_c2r_1d(
        FRAMES_PER_WINDOW, (*userData)->complexData, (*userData)->outputWindow, FFTW_ESTIMATE);

    /*
    firstLoop is needed since the pitch shift doesn't have enough data
    during the first execution of the callback function
    */
    (*userData)->firstLoop = true;

    // can be used for testing
    (*userData)->test = true;

    // saves which filter is active and the pitch shift factor
    (*userData)->aFilter = (activeFilter*)malloc(sizeof(activeFilter));
    (*userData)->aFilter->pitchShift = false;
    (*userData)->aFilter->shiftFactor = 1.0;
    (*userData)->aFilter->echo = false;
    (*userData)->aFilter->robot = false;


    checkAllocation(*userData);
}

/*
Frees disk space used by an instanze of type callbackData
*/
void freeUserData(callbackData* userData) {
    free(userData->leftInBuffer);
    fftw_free(userData->inputWindow);
    fftw_free(userData->complexData);
    fftw_free(userData->outputWindow);
    free(userData->leftOutBuffer);
    free(userData->hannWindow);

    free(userData->carrierSignal->signal);
    free(userData->carrierSignal);

    free(userData->echoData->buffer);
    free(userData->echoData);

    fftw_destroy_plan(userData->plan);
    fftw_destroy_plan(userData->iPlan);

    free(userData->aFilter);

    free(userData);

    std::cout << std::endl << "userData freed successfully" << std::endl;
}

/*
Creates a Hann window which is needed for the discrete Fourier transform
*/
void createHannWindow(double* hannWindow) {
    for (int i = 0; i < (FRAMES_PER_WINDOW*CHANNELS); ++i) {
        hannWindow[i] = 0.5 * (1 - cos(2 * M_PI * i / (FRAMES_PER_WINDOW-1)));
    }
}

/*
Initializes a buffer with only zeros
*/
void zeroBuffer(double* buffer) {
    for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
        buffer[i] = 0;
    }

}

/*
Checks if all arrays of userData are allocated
*/
void checkAllocation(callbackData* userData) {
    if (userData->leftInBuffer == NULL || userData->inputWindow == NULL || userData->complexData == NULL ||
        userData->outputWindow == NULL || userData->leftOutBuffer == NULL || userData->hannWindow == NULL ||
        userData->carrierSignal == NULL || userData->carrierSignal->signal == NULL || userData->aFilter == NULL ||
        userData->echoData == NULL || userData->echoData->buffer == NULL) {
        std::cout << "Allocation problem" << std::endl;
        exit(1);
    }
}
