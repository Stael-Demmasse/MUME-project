#ifndef FILTERROBOT_H
#define FILTERROBOT_H

#include "Structs.h"

void createCarrierSignal(carrierSignalInfos* carrierSignal);
void robotVoice(float* out, carrierSignalInfos* carrierSignal, unsigned long framesPerBuffer);

#endif
