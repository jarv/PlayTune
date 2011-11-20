/* 
 * Copyright (c) 2011 John Jarvis
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "playtune.h"

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define output_toggle(port,pin) port ^= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

#if defined( __AVR_ATtiny25__ ) | \
     defined( __AVR_ATtiny45__ ) | \
     defined( __AVR_ATtiny85__ )
#  define ATTINY
#endif


#if defined( __AVR_ATmega328P__ ) | \
    defined( __AVR_ATmega168__ )
# define ATMEGA
#endif 


/* PlayTune::PlayTune
   A tick represents one call to PlayNote, the amount of time
   to rest between ticks is left to the caller.

    timer: corresponds to one of the three
        AVR timers, valid values are 0,1 or 2.

    prescale: needed to setup the waveform, valid
        values are 1,8,64,256 or 1024.
*/

uint16_t PlayTune::pause = 10;

PlayTune::PlayTune( uint8_t timer, uint16_t prescale, 
        const uint8_t *notes, const uint8_t *delays,
        uint16_t notes_len, uint16_t delays_len)
{


    timer_ = timer;

    prescale_ = prescale;
    notes_ = notes;
    notes_len_ = notes_len;
    delays_ = delays;
    delays_len_ = delays_len;

    timeleft_ = 0;
    pos_ = 0;

    isFinished_ = false;
    // initialize the timer for
    // CTC mode and set corresponding pin for ouput

    switch(timer_) {
        case TIMER0:
#if defined(ATMEGA)
            TCCR0A = 0;
            TCCR0B = 0;
            TCCR0A |= (1<<WGM01);  // configure timer 1 for CTC mode
            TCCR0A |= (1<<COM0A0); // toggle OC0A (PD6/arduino 6) on 
                                   // compare match
            DDRD |= (1<<PD6);     // set PD6 for ouput
#elif defined(ATTINY)
            TCCR0A = 0;
            TCCR0B = 0;
            TCCR0A |= (1<<WGM01);  // configure timer 1 for CTC mode
            TCCR0A |= (1<<COM0A0); // toggle OC0A (PD6/arduino 6) on 
                                   // compare match
            DDRB |= (1<<PB0); 
#else
    #error No target for timer0 setup
#endif
            break;
        
        case TIMER1:
#if defined(ATMEGA)
            TIMSK1 |= (1<<OCIE1B);  // timer1 compare B match interrupt enabled
            sei();
            
            TCCR1B = 0;
            TCCR1A = 0;

            TCCR1B |= (1<<WGM12);  // configure timer 2 for CTC mode
            TCCR1A |= (1<<COM1A0); // toggle OC1A (PB1/arduino 9) on 
                                   //compare match
            DDRB |= (1<<PB1);     // set PB1 for output
            OCR1B = 255;
#elif defined(ATTINY)
            TCCR1 = 0;
            TCCR1 |= (1<<CS13) | (1<<CS10); // clk/256
            TCCR1 |= (1<<CTC1); // configure for CTC mode
            TCCR1 |= (1<<COM1A0); // toggle OC1A on compare match
            DDRB |= (1<<PB1);
#else
    #error No target for timer1 setup
#endif
            break;
#if defined(ATMEGA)
        case TIMER2:
    
            TCCR2A = 0;
            TCCR2B = 0;

            TCCR2A |= (1<<WGM21); // configure timer 3 for CTC mode
            TCCR2A |= (1<<COM2A0); // toggle OC2A (PB3/arduino 11) 
                                   // on compare match
            DDRB |= (1<<PB3);     // set PB3 for output
            break;
#endif
    };
}


/* PlayTune::playNote()
   Must be called every tick!
   Will set the corresponding register for the configured
   timer to the top value specified in the notes array.
   Will stop the timer if there is rest.
   
   Returns TRUE on success, FALSE if not.
*/

uint8_t
PlayTune::playNote(void)
{
    if (isFinished_ == true) {
        return(0);
    } else if (pos_ >= notes_len_) {
        // reached the end of the song, reset
        // position, turn off timer mark as finished
        turnOff();
        pos_ = 0;
        isFinished_ = true;
    } else if (timeleft_ == 0) {
        // play a new note or a rest
        timeleft_ = pgm_read_byte(&delays_[pos_]);
        uint8_t cur_note = pgm_read_byte(&notes_[pos_]);
        if (cur_note) {
            if (cur_note == last_note_) {
                // playing the same note, going to introduce
                // a small pause between the notes
                turnOff();
#if defined(ATMEGA)
                _delay_ms(PlayTune::pause);
#elif defined(ATTINY)
                // :(
                uint16_t cnt = 60000;
                while (cnt--) {
                    __asm("NOP");
                }
#else
    #error No target for note pause
#endif
            }
            turnOn();
            switch(timer_) {
                case TIMER0:
#if defined(ATMEGA) | defined(ATTINY)
                    OCR0A=cur_note;
#else
    #error No target for timer0 setup
#endif

                    break;
                case TIMER1:
#if defined(ATMEGA)
                    OCR1A=cur_note;
#elif defined(ATTINY)
                    OCR1C=cur_note;
#else
    #error No target for timer1 setup
#endif
                    break;
#if defined(ATMEGA)

                case TIMER2:
                    OCR2A=cur_note;
                    break;
#endif
            }
            last_note_ = cur_note;
        } else {
            turnOff();
        }
        // remove one tick from the delay counter
        timeleft_--;
        // advance to the next position
        pos_++;
    } else {
        // remove one tick from the delay counter
        timeleft_--;
    }
    return(1);
}


uint8_t
PlayTune::turnOff()
{
    switch(timer_) {
        case TIMER0:
#if defined(ATMEGA) | defined(ATTINY)
            TCCR0B &= ~(1<<CS02) & ~(1<<CS01) & ~(1<<CS00);
#else
    #error No target for timer0 setup
#endif
            break;
        case TIMER1:
#if defined(ATMEGA)
            TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10); 
#elif defined(ATTINY)
            TCCR1 &= ~(1<<CS10) & ~(1<<CS11) & ~(1<<CS12) & ~(1<<CS13);
#else
    #error No target for timer1 setup
#endif
            break;
#if defined(ATMEGA)
        case TIMER2:
            TCCR2B &= ~(1<<CS22) & ~(1<<CS21) & ~(1<<CS20);
            break;
#endif
        default:
            return(0);
        }
    return(1);
}


uint8_t
PlayTune::turnOn()
{
    switch(timer_) {
        case TIMER0:
            switch(prescale_) {
#if defined(ATMEGA) | defined(ATTINY)
                case PS_1:
                    TCCR0B |= (1<<CS00);
                    break;
                case PS_8:
                    TCCR0B |= (1<<CS01);
                    break;
                case PS_64:
                    TCCR0B |= (1<<CS01) | (1<<CS00);
                    break;
                case PS_256:
                    TCCR0B |= (1<<CS02);
                    break;
                case PS_1024:
                    TCCR0B |= (1<<CS02) | (1<<CS00);
                    break;
#else
    #error No target for timer0 setup
#endif
                default:
                    return(0);
            }
            break;
        case TIMER1:
            switch(prescale_) {
#if defined(ATMEGA)
                case PS_1:
                    TCCR1B |= (1<<CS10);
                    break;
                case PS_8:
                    TCCR1B |= (1<<CS11);
                    break;
                case PS_64:
                    TCCR1B |= (1<<CS11) | (1<<CS10);
                    break;
                case PS_256:
                    TCCR1B |= (1<<CS12);
                    break;
                case PS_1024:
                    TCCR1B |= (1<<CS12) | (1<CS10);
                    break;
#elif defined(ATTINY)
                case PS_1:
                    TCCR1 |= (1<<CS10);
                    break;
                case PS_8:
                    TCCR1 |= (1<<CS12);
                    break;
                case PS_64:
                    TCCR1 |= (1<<CS12) | (1<<CS11) | (1<<CS10);
                    break;
                case PS_256:
                    TCCR1 |= (1<<CS13) | (1<<CS10);
                    break;
                case PS_1024:
                    TCCR1 |= (1<<CS13) | (1<CS11) | (1<<CS10);
                    break;
#else
    #error No target for timer1 setup
#endif
                default:
                    return(0);
            }
            break;
#if defined(ATMEGA)
        case TIMER2:
            switch(prescale_) {
                case PS_1:
                    TCCR2B |= (1<<CS20);
                    break;
                case PS_8:
                    TCCR2B |= (1<<CS21);
                    break;
                case PS_64:
                    TCCR2B |= (1<<CS22);
                    break;
                case PS_256:
                    TCCR2B |= (1<<CS22) | (1<<CS21);
                    break;
                case PS_1024:
                    TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
                    break;
                default:
                    return(0);
                }
            break;
#endif
        default:
            return(0);
        return(1);
    }
    return(0);
}

uint8_t 
PlayTune::isPlaying(void)
{
    if (isFinished_) {
        return(0);
    } else {
        return (1);
    }
}
#if defined(ATMEGA)
ISR(TIMER1_COMPB_vect)
{
    TCNT1=0;
}
#endif

    // -- TIMER0
    //set_output(DDRD, PD6); 
    //TCCR0A |= (1<<WGM01);  // configure timer 1 for CTC mode
    //TCCR0A |= (1<<COM0A0); // toggle OC0A (PD6/arduino 6) on compare match
    //TCCR0B |= (1<<CS02);   // clk/256 prescale
    //OCR0A = 100;

    // -- TIMER1
    //set_output(DDRB, PB1);
    //TCCR1B |= (1<<WGM12); // CTC 
    //TCCR1A |= (1<<COM1A0); // toggle OC1A (PB1/arduino 9) on compare match
    //TCCR1B |= (1<<CS12); // /256 prescale
    //OCR1A = 100;

    // -- TIMER2
    //set_output(DDRB, PB3);
    //TCCR2A |= (1<<COM2A0); // toggle OC2A (PB3/arduino 11) on compare match
    //TCCR2A |= (1<<WGM21); // CTC
    //TCCR2B |= (1<<CS22) | (1<<CS21); // clk/256 prescale
    //OCR2A = 100;



