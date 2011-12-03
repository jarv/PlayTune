#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "playtune.h"
#include "songs/gotafriend.h"
int main(void) 
{
    // set everything low that we aren't using
    DDRB |= (1<<PB2) | (1<<PB4) | (1<<PB3) | (1<<PB5);
    PORTB |= (1<<PB2) | (1<<PB4) | (1<<PB3) | (1<<PB5);
    PlayTune p0(0,PART0);
    PlayTune p1(1,PART1);
    while ( p0.isPlaying() || p1.isPlaying() ) {
    
        p0.playNote(); 
        p1.playNote(); 
        _delay_ms(80);
    } 
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode(); 
    return(0);
}

