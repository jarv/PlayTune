#ifndef MSPACMAN_GAME_START
#define MSPACMAN_GAME_START
#include <stdint.h>
#include <avr/pgmspace.h>

#define MSPACMAN_GAME_START0 64,mspacman_game_start0_notes,mspacman_game_start0_delays,sizeof(mspacman_game_start0_notes)/sizeof(*(mspacman_game_start0_notes)),sizeof(mspacman_game_start0_delays)/sizeof(*(mspacman_game_start0_delays))
#define MSPACMAN_GAME_START1 128,mspacman_game_start1_notes,mspacman_game_start1_delays,sizeof(mspacman_game_start1_notes)/sizeof(*(mspacman_game_start1_notes)),sizeof(mspacman_game_start1_delays)/sizeof(*(mspacman_game_start1_delays))
/*

    Part: 0
    Freq: 8000000
    Number of voices: 1
    Note length: 27
    Delay length: 27

    Part: 1
    Freq: 8000000
    Number of voices: 1
    Note length: 23
    Delay length: 23

*/
const uint8_t mspacman_game_start0_notes[] PROGMEM = {
0,118,105,0,94,88,70,79,66,70,66,59,70,79,66,70,66,59,70,66,59,52,46,44,46,44,0
};
const uint8_t mspacman_game_start0_delays[] PROGMEM = {
16,5,5,1,5,16,16,16,16,8,8,8,8,16,16,8,8,8,8,8,8,8,8,16,16,16,16
};

const uint8_t mspacman_game_start1_notes[] PROGMEM = {
0,178,0,238,0,178,0,158,141,133,158,141,158,178,141,158,178,189,158,178,238,178,0
};
const uint8_t mspacman_game_start1_delays[] PROGMEM = {
32,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,16,16,16,16
};

#endif
