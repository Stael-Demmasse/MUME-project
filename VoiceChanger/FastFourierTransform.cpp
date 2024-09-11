#include <cmath>

#include "FastFourierTransform.h"
#include "Constants.h"

/*
Fills a window for the DFT (discrete Fourier transform).
The first half gets filled with the previous input while the second is
being filled with the current input.
*/
void fillWindow(float* inBuffer, callbackData* data) {
    for (int i = 0; i < (FRAMES_PER_WINDOW*CHANNELS); ++i) {
        if (i < FRAMES_PER_BUFFER*CHANNELS)
            data->inputWindow[i] = data->leftInBuffer[i] * data->hannWindow[i];
        else
            data->inputWindow[i] = inBuffer[i-FRAMES_PER_BUFFER] * data->hannWindow[i];
    }
}

/*
Normalization is needed after the DFT.
*/
void normalizeWindow(callbackData* data) {
    for (int i = 0; i < FRAMES_PER_WINDOW; i++) {
        data->outputWindow[i] = data->outputWindow[i] / (FRAMES_PER_WINDOW);
    }
}
