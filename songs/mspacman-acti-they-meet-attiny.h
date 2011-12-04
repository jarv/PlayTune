#ifndef MSPACMAN_ACTI_THEY_MEET
#define MSPACMAN_ACTI_THEY_MEET
#include <stdint.h>
#include <avr/pgmspace.h>

#define MSPACMAN_ACTI_THEY_MEET0 64,mspacman_acti_they_meet0_notes,mspacman_acti_they_meet0_delays,sizeof(mspacman_acti_they_meet0_notes)/sizeof(*(mspacman_acti_they_meet0_notes)),sizeof(mspacman_acti_they_meet0_delays)/sizeof(*(mspacman_acti_they_meet0_delays))
#define MSPACMAN_ACTI_THEY_MEET1 128,mspacman_acti_they_meet1_notes,mspacman_acti_they_meet1_delays,sizeof(mspacman_acti_they_meet1_notes)/sizeof(*(mspacman_acti_they_meet1_notes)),sizeof(mspacman_acti_they_meet1_delays)/sizeof(*(mspacman_acti_they_meet1_delays))
/*

    Part: 0
    Freq: 8000000
    Number of voices: 1
    Note length: 47
    Delay length: 47

    Part: 1
    Freq: 8000000
    Number of voices: 1
    Note length: 43
    Delay length: 43

*/
const uint8_t mspacman_acti_they_meet0_notes[] PROGMEM = {
46,41,46,55,74,62,59,55,62,55,62,74,46,41,46,55,74,62,74,37,31,28,31,27,31,27,31,37,41,46,41,46,41,37,41,46,55,62,55,62,74,62,74,83,94,112,0
};
const uint8_t mspacman_acti_they_meet0_delays[] PROGMEM = {
2,2,2,4,2,2,2,8,2,2,2,2,2,2,2,4,2,4,10,2,2,2,1,3,1,3,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,12,4
};

const uint8_t mspacman_acti_they_meet1_notes[] PROGMEM = {
0,112,0,112,112,0,112,0,112,112,0,0,0,112,0,112,112,0,149,0,149,149,0,0,112,126,141,149,112,0,94,0,83,0,74,0,62,74,83,94,112,0,0
};
const uint8_t mspacman_acti_they_meet1_delays[] PROGMEM = {
2,2,2,2,2,8,2,2,2,2,2,4,2,2,2,2,2,8,2,2,2,2,2,4,8,8,8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4
};

#endif
