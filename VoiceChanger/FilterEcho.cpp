#include <cmath>

#include "FilterEcho.h"
#include "Constants.h"

/*
Creates the Echo effect.
*/
void echoEffect(float* in, float* out, echoDataInfos* echoData, unsigned long framesPerBuffer){

    for(unsigned long i = 0; i < framesPerBuffer; ++i) {
        float inputSample = in[i];
        float delayedSample = echoData->buffer[echoData->writeIndex];

        // Output is the sum of the input and the delayed signal
        out[i] = inputSample + ECHO_AMPLITUDE * delayedSample;

        // Store the current sample in the delay buffer
        echoData->buffer[echoData->writeIndex] = inputSample;

        // Increment and wrap the write index
        echoData->writeIndex = (echoData->writeIndex + 1) % echoData->bufferSize;
    }
}
