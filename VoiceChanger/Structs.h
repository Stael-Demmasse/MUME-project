#ifndef STRUCTS_H
#define STRUCTS_H

#include <fftw3.h>

/*
Struct saves data used by the robot filter.
*/
typedef struct {
    double* signal;
    int currentBuffer;
    int maxBuffers;
} carrierSignalInfos;

/*
Struct saves data used by the echo filter.
*/
typedef struct {
    float* buffer;  // Buffer für den Echo-Effekt
    int bufferSize;
    int writeIndex;
} echoDataInfos;

/*
Struct which holds the active filters and the pitch shift factor
*/
typedef struct {
    bool pitchShift;
    double shiftFactor;
    bool echo;
    bool robot;
} activeFilter;

/*
Struct saves data used by portaudio.
*/
typedef struct {
    double* leftInBuffer;               // saves input from last interation (windowing)
    double* inputWindow;                // input window for the DFT
    fftw_complex* complexData;          // output from DFT
    double* outputWindow;               // output from inverse DFT
    double* leftOutBuffer;              // saves right half of window for next interation (windowing)
    double* hannWindow;                 // window function
    carrierSignalInfos* carrierSignal;  // carrier signal for robot filter
    echoDataInfos* echoData;            // data for the echo filter
    fftw_plan plan;                     // DFT plan
    fftw_plan iPlan;                    // inverse DFT plan
    bool firstLoop;                     // DFT dosn't work during first iteration of portaudio
    bool test;
    activeFilter* aFilter;              // saves which filter is currently aktive and shift factor
} callbackData;



#endif
