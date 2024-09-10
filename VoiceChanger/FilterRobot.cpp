#include <cmath>

#include "FilterRobot.h"
#include "Constants.h"


/*
Creates a sinus wave which is maxBuffers buffers big and which has a
frequency of ROBOT_VOICE_CARRIER_LENGTH.
*/
void createCarrierSignal(carrierSignalInfos* carrierSignal) {
    for (int i = 0; i < (FRAMES_PER_BUFFER*carrierSignal->maxBuffers*CHANNELS); ++i) {
        carrierSignal->signal[i] = sin(2 * M_PI * ROBOT_VOICE_CARRIER_LENGTH * i /(FRAMES_PER_BUFFER-1));
    }
}

/*
Multiplies the carrier signal onto the output buffer for a robot voice effect.
*/
void robotVoice(float* out, carrierSignalInfos* carrierSignal, unsigned long framesPerBuffer) {
    for (int i = 0; i < framesPerBuffer; ++i) {
        out[i] = out[i] * carrierSignal->signal[i];
    }
    carrierSignal->currentBuffer = (carrierSignal->currentBuffer + 1) % carrierSignal->maxBuffers;
}
