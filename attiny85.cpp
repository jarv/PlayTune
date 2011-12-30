#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "playtune.h"
#include "songs/canyoufeel.h"

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define output_toggle(port,pin) port ^= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)
#define read_value(portdir,pin) ( portdir & (1<<pin) )
#define LED PB3
int main(void) 
{
    // set everything low that we aren't using
    DDRB |= (1<<PB2) | (1<<PB4) | (1<<PB3) | (1<<PB5);
    PORTB &= ~(1<<PB2) | ~(1<<PB4) | ~(1<<PB3) | ~(1<<PB5);
    PlayTune p0(0,CANYOUFEEL0);
    PlayTune p1(1,CANYOUFEEL1);
    while ( p0.isPlaying() || p1.isPlaying() ) {
        p0.playNote();
        p1.playNote();
        _delay_ms(40);
    }
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
    return(0);

    
}
