#ifndef mspacman-acti-they-meet
#define mspacman-acti-they-meet
#include <stdint.h>
#include <avr/pgmspace.h>

#define PART0 256,p0_notes,p0_delays,sizeof(p0_notes)/sizeof(*(p0_notes)),sizeof(p0_delays)/sizeof(*(p0_delays))
#define PART1 1024,p1_notes,p1_delays,sizeof(p1_notes)/sizeof(*(p1_notes)),sizeof(p1_delays)/sizeof(*(p1_delays))
/*

    Part: 0
    Freq: 16000000
    Number of voices: 1
    Note length: 47
    Delay length: 47

    Part: 1
    Freq: 16000000
    Number of voices: 1
    Note length: 43
    Delay length: 43

*/
const uint8_t p0_notes[] PROGMEM = {
46,41,46,55,74,62,59,55,62,55,62,74,46,41,46,55,74,62,74,37,31,29,31,27,31,27,31,37,41,46,41,46,41,37,41,46,55,62,55,62,74,62,74,83,94,112,0
};
const uint8_t p0_delays[] PROGMEM = {
2,2,2,4,2,2,2,8,2,2,2,2,2,2,2,4,2,4,10,2,2,2,1,3,1,3,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,12,4
};

const uint8_t p1_notes[] PROGMEM = {
0,55,0,55,55,0,55,0,55,55,0,0,0,55,0,55,55,0,74,0,74,74,0,0,55,62,70,74,55,0,46,0,41,0,37,0,31,37,41,46,55,0,0
};
const uint8_t p1_delays[] PROGMEM = {
2,2,2,2,2,8,2,2,2,2,2,4,2,2,2,2,2,8,2,2,2,2,2,4,8,8,8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4
};

#endif
