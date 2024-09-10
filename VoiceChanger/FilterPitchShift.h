#ifndef FILTERPITCHSHIFT_H
#define FILTERPITCHSHIFT_H

#include "Structs.h"

void pitchShiftEffect(float* in, float* out, callbackData* callData, unsigned long framesPerBuffer);
void shift(callbackData* userData);

#endif
