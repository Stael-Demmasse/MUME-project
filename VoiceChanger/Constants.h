#ifndef CONSTANTS_H
#define CONSTANTS_H


// General constants
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 2048
#define FRAMES_PER_WINDOW (FRAMES_PER_BUFFER*2)
#define CHANNELS 1

// Robot filter constants
#define ROBOT_VOICE_MAX_BUFFERS 1
#define ROBOT_VOICE_CARRIER_LENGTH 2            //regulates intensity of the robot filter

// Echo filter constants
#define ECHO_DELAY 0.2 // Delay in seconds
#define ECHO_AMPLITUDE 0.6

// Pitch shift filter constants
#define MAX_SHIFT_UP 2


#endif
