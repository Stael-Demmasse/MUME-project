#ifndef DATA_H
#define DATA_H

#include "Structs.h"

void initializeUserData(callbackData** userData);
void freeUserData(callbackData* userData);
void createHannWindow(double* hannWindow);
void zeroBuffer(double* buffer);
void checkAllocation(callbackData* userData);


#endif
