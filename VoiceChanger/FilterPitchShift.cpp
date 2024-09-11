#include "FilterPitchShift.h"

#include <cmath>
#include <string.h>

#include "Constants.h"
#include "FastFourierTransform.h"

/*
Creates the pitch shift by executing a dft and shifting the complex output.
*/
void pitchShiftEffect(float* in, float* out, callbackData* data, unsigned long framesPerBuffer) {
    fillWindow(in, data);

    fftw_execute(data->plan);

    shift(data);
    fftw_execute(data->iPlan);

    //Required after a discrete Fourier transform
    normalizeWindow(data);


    /*
     writes the first half of the window into the out Buffer
     safes the second half into callbackData->leftOutBuffer
    */
    for (int i = 0; i < (framesPerBuffer*CHANNELS); ++i) {
        out[i] = data->outputWindow[i] + data->leftOutBuffer[i];
        data->leftOutBuffer[i] = data->outputWindow[i+(framesPerBuffer*CHANNELS)];
    }

    /*
     safes the input Buffer in callbackData->leftInBuffer
     which is needed for the windowing
    */
    for (int i = 0; i < (framesPerBuffer*CHANNELS); ++i) {
        data->leftInBuffer[i] = in[i];
    }
}

/*
Performs the pitch shift
*/
void shift(callbackData* userData) {
    double pitchShift = userData->aFilter->shiftFactor;
    int complexArrayLength = FRAMES_PER_WINDOW/2 + 1;
    int maxLentgth = FRAMES_PER_WINDOW;

    double real;
    double imag;
    double magnitude;
    double phase;
    double temp;
    double lastPhase[complexArrayLength/2];
    double sumPhase[maxLentgth/2+1];
    long qpd;
    long osamp = 2;
    long stepSize = maxLentgth / osamp;
    double expct = 2. * M_PI * (double)stepSize / (double)maxLentgth;
    double freqPerBin = (double) SAMPLE_RATE / (double) maxLentgth;

    double magn[complexArrayLength/2];
    double freq[complexArrayLength/2];
    double shiftMagn[maxLentgth];
    double shiftFreq[maxLentgth];
    int index;

    memset(lastPhase, 0, sizeof(double) * complexArrayLength/2);
    memset(sumPhase, 0, sizeof(double) * maxLentgth/2+1);
    memset(magn, 0, sizeof(double) * complexArrayLength/2);
    memset(freq, 0, sizeof(double) * complexArrayLength/2);
    memset(shiftMagn, 0, sizeof(double) * maxLentgth);
    memset(shiftFreq, 0, sizeof(double) * maxLentgth);



    //Analyse
    for (int i = 0; i < (complexArrayLength/2); ++i) {
        real = *(userData->complexData[2*i]);
        imag = *(userData->complexData[2*i + 1]);


        magnitude = 2 * sqrt(real*real + imag*imag);
        phase = atan2(imag, real);

        temp = phase - lastPhase[i];
        lastPhase[i] = phase;

        temp -= (double)i*expct;

        qpd = temp / M_PI;
        if (qpd >= 0)
            qpd += (qpd & 1);   //bit operation (ouputs one if qpd is uneven)
        else
            qpd -= (qpd & 1);
        temp -= M_PI * (double)qpd;

        temp = osamp * temp / (2. * M_PI);

        temp = (double)i * freqPerBin + temp * freqPerBin;

        magn[i] = magnitude;
        freq[i] = temp;
    }


    //Shift
    for (int i = 0; i < complexArrayLength/2; ++i) {
        index = i * pitchShift;
        if (index <= maxLentgth) {
            shiftMagn[index] += magn[i];
            shiftFreq[index] = freq[i] * pitchShift;
        }

    }

    //Synthesis
    for (int i = 0; i < complexArrayLength/2; ++i) {
        magnitude = shiftMagn[i];
        temp = shiftFreq[i];

        temp -= (double)i * freqPerBin;

        temp /= freqPerBin;

        temp = 2. * M_PI * temp / osamp;

        temp += (double)i * expct;

        sumPhase[i] += temp;
        phase = sumPhase[i];


        *(userData->complexData)[2*i] = magnitude * cos(phase);
        *(userData->complexData)[2*i + 1] = magnitude * sin(phase);

    }

}
