#ifndef STD_END
#define STD_END
#include <stdint.h>
#include <avr/pgmspace.h>

#define STD_END0 128,std_end0_notes,std_end0_delays,sizeof(std_end0_notes)/sizeof(*(std_end0_notes)),sizeof(std_end0_delays)/sizeof(*(std_end0_delays))
/*

    Part: 0
    Freq: 8000000
    Number of voices: 1
    Note length: 68
    Delay length: 68

*/
const uint8_t std_end0_notes[] PROGMEM = {
0,158,0,141,126,0,118,126,141,118,126,0,158,141,158,126,141,158,88,94,0,118,0,0,118,0,105,0,0,94,105,0,94,105,0,118,94,0,94,105,0,0,105,112,0,105,94,0,94,105,0,105,118,0,94,105,0,0,105,0,118,0,118,126,0,105,126,158
};
const uint8_t std_end0_delays[] PROGMEM = {
4,7,1,8,6,2,16,16,17,16,39,2,8,8,9,16,17,16,16,36,4,3,1,4,7,1,10,2,4,16,7,2,8,8,1,9,4,4,8,5,1,2,8,7,1,24,4,4,8,5,4,8,7,1,8,7,1,2,7,1,4,4,8,7,1,8,8,4
};

#endif
