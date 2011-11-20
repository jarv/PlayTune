#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "songs/gotafriend.h"
#include "playtune.h"

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define output_toggle(port,pin) port ^= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)
#define read_value(portdir,pin) ( portdir & (1<<pin) )
#define LED PB5
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)))

#define p0 p0
#define p1 p1
#define p2 p2


int main(void) {

    set_output(DDRB, LED);
    output_low(PORTB, LED);
    PlayTune p0(0);
    PlayTune p1(1);
              
    while ( p0.isPlaying() || p1.isPlaying() ) {
        p0.playNote();
        p1.playNote();
        _delay_ms(500); 
        //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        //sleep_mode();
    }
    return(0);
}

