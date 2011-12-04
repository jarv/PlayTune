#ifndef MSPACMAN_ACTII_THE_CHASE
#define MSPACMAN_ACTII_THE_CHASE
#include <stdint.h>
#include <avr/pgmspace.h>

#define MSPACMAN_ACTII_THE_CHASE0 64,mspacman_actii_the_chase0_notes,mspacman_actii_the_chase0_delays,sizeof(mspacman_actii_the_chase0_notes)/sizeof(*(mspacman_actii_the_chase0_notes)),sizeof(mspacman_actii_the_chase0_delays)/sizeof(*(mspacman_actii_the_chase0_delays))
#define MSPACMAN_ACTII_THE_CHASE1 128,mspacman_actii_the_chase1_notes,mspacman_actii_the_chase1_delays,sizeof(mspacman_actii_the_chase1_notes)/sizeof(*(mspacman_actii_the_chase1_notes)),sizeof(mspacman_actii_the_chase1_delays)/sizeof(*(mspacman_actii_the_chase1_delays))
/*

    Part: 0
    Freq: 8000000
    Number of voices: 1
    Note length: 110
    Delay length: 110

    Part: 1
    Freq: 8000000
    Number of voices: 1
    Note length: 149
    Delay length: 149

*/
const uint8_t mspacman_actii_the_chase0_notes[] PROGMEM = {
0,79,62,59,55,52,55,52,62,52,46,44,46,44,31,34,39,44,46,44,70,62,59,79,46,44,41,39,59,52,49,46,59,46,59,79,94,88,83,79,66,62,88,44,46,62,59,46,52,66,62,52,44,34,39,44,52,62,70,79,59,52,49,46,70,66,62,52,46,44,41,39,59,52,49,46,59,46,59,79,83,79,70,74,70,0,44,46,52,59,52,46,46,52,59,79,118,105,70,79,62,44,70,62,59,79,0,59,0,0
};
const uint8_t mspacman_actii_the_chase0_delays[] PROGMEM = {
4,4,2,2,2,4,2,4,2,2,2,4,2,4,2,2,2,2,2,4,1,1,12,4,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,8,8,12,4,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,6,2,2,8,8,15,1,2,2,2,4,2,4,2,2,2,4,2,2,2,8,2,2,2,2,4,3,1,3,1,4
};

const uint8_t mspacman_actii_the_chase1_notes[] PROGMEM = {
0,62,66,66,70,70,74,79,0,88,94,99,105,105,112,118,0,158,0,158,0,158,141,126,118,0,94,0,158,0,94,0,118,0,94,0,158,0,94,0,118,0,94,0,158,0,94,0,94,0,79,0,99,0,83,0,105,0,88,0,158,0,88,0,105,0,88,0,158,0,88,0,105,0,88,0,158,0,88,0,88,0,0,105,0,0,94,88,83,79,79,94,105,158,118,0,94,0,158,0,94,0,118,0,94,0,158,0,94,0,141,0,88,0,94,0,112,0,105,0,126,0,141,0,158,0,178,0,0,88,0,94,0,0,178,0,0,88,0,0,88,0,118,0,158,0,118,0,0
};
const uint8_t mspacman_actii_the_chase1_delays[] PROGMEM = {
8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,6,2,3,1,2,1,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,4,3,1,4,2,2,2,2,2,2,2,2,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,8,3,1,3,1,8,3,1,4,3,1,4,3,1,3,1,3,1,3,1,4
};

#endif
