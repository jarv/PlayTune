#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "playtune.h"
#include "songs/mspacman-acti-they-meet-attiny.h"
#include "songs/mspacman-game-start-attiny.h"
#include "songs/mspacman-actii-the-chase-attiny.h"


int main(void) 
{
    // setup interrupt
    GIMSK |= (1<<INT0); // INT0 enabled for interrupts
    //MCUCR |= (1<<ISC00); // any logic change on INT0
    sei();

    while(1) {
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode(); 
    }
    return(0);
}

volatile uint8_t tune = 0;
ISR (INT0_vect)
{
    PlayTune theymeet0(0,MSPACMAN_ACTI_THEY_MEET0);
    PlayTune theymeet1(1,MSPACMAN_ACTI_THEY_MEET1);
    PlayTune gamestart0(0,MSPACMAN_GAME_START0);
    PlayTune gamestart1(1,MSPACMAN_GAME_START1);
    PlayTune thechase0(0,MSPACMAN_ACTII_THE_CHASE0);
    PlayTune thechase1(1,MSPACMAN_ACTII_THE_CHASE1);
    
    switch(tune) {

        case 1:
            while ( theymeet0.isPlaying() || theymeet1.isPlaying() ) {
            
                theymeet0.playNote(); 
                theymeet1.playNote(); 
                _delay_ms(65);
            } 
            break;
        case 2:
            while ( gamestart0.isPlaying() || gamestart1.isPlaying() ) {
            
                gamestart0.playNote(); 
                gamestart1.playNote(); 
                _delay_ms(15);
            } 
            break; 

        case 3:
            while ( thechase0.isPlaying() || thechase1.isPlaying() ) {
            
                thechase0.playNote(); 
                thechase1.playNote(); 
                _delay_ms(65);
            } 
            break;
    } 

    if (tune == 3) {
        tune = 1;
    } else {
        tune++;
    } 
}
