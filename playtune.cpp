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

#if defined( __AVR_AT90USB1287__)
#define AT90USB
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
#if defined(ATMEGA) || defined(AT90USB)
    #if defined(ATMEGA)
            DDRD |= (1<<PORTD6);     // set PORTD6 for ouput
    #elif defined(AT90USB)
            DDRB |= (1<<PORTB7);   // set PORTB7 for output
    #else
        #error No target for timer0 output pin
    #endif

            TCCR0A = 0;
            TCCR0B = 0;
            TCCR0A |= (1<<WGM01);  // configure timer 1 for CTC mode
            TCCR0A |= (1<<COM0A0); // toggle OC0A (PORTD6/arduino 6) on 
                                   // compare match
#elif defined(ATTINY)
            TCCR0A = 0;
            TCCR0B = 0;
            TCCR0A |= (1<<WGM01);  // configure timer 1 for CTC mode
            TCCR0A |= (1<<COM0A0); // toggle OC0A (PORTD6/arduino 6) on 
                                   // compare match
            DDRB |= (1<<PORTB0); 
#else
    #error No target for timer0 setup
#endif
            break;
        
        case TIMER1:
#if defined(ATMEGA) || defined(AT90USB)
            TIMSK1 |= (1<<OCIE1B);  // timer1 compare B match interrupt enabled
            sei();
            
            TCCR1B = 0;
            TCCR1A = 0;

            TCCR1B |= (1<<WGM12);  // configure timer 2 for CTC mode
            TCCR1A |= (1<<COM1A0); // toggle OC1A (PORTB1/arduino 9) on 
                                   //compare match
    #if defined(ATMEGA)
            DDRB |= (1<<PORTB1);     // set PORTB1 for output
    #elif defined(AT90USB)
            DDRB |= (1<<PORTB5);   // set PORTB5 for output
    #else
        #error No target for timer1 output pin
    #endif
            OCR1B = 255;
#elif defined(ATTINY)
            TCCR1 = 0;
            TCCR1 |= (1<<CS13) | (1<<CS10); // clk/256
            TCCR1 |= (1<<CTC1); // configure for CTC mode
            TCCR1 |= (1<<COM1A0); // toggle OC1A on compare match
            DDRB |= (1<<PORTB1);
#else
    #error No target for timer1 setup
#endif
            break;
#if defined(ATMEGA) || defined(AT90USB)
        case TIMER2:
    
            TCCR2A = 0;
            TCCR2B = 0;

            TCCR2A |= (1<<WGM21); // configure timer 3 for CTC mode
            TCCR2A |= (1<<COM2A0); // toggle OC2A (PORTB3/arduino 11) 
                                   // on compare match
    #if defined(ATMEGA)
            DDRB |= (1<<PORTB3);     // set PORTB3 for output
    #elif defined(AT90USB)
            DDRB |= (1<<PORTB4);
    #else
        #error No target for timer2 output pin
    #endif
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
    } else if (timeleft_ == 0) {
        // check to see if the song is
        // over, if it is return.
        if (pos_ >= notes_len_) {
            turnOff();
            pos_ = 0;
            isFinished_ = true;
            return(1);
        }
        // play a new note or a rest
        timeleft_ = pgm_read_byte(&delays_[pos_]);
        uint8_t cur_note = pgm_read_byte(&notes_[pos_]);
        if (cur_note) {
            if (cur_note == last_note_) {
                // playing the same note, going to introduce
                // a small pause between the notes
                turnOff();
                //_delay_ms(PlayTune::pause);
                for (uint16_t i=0; i<PlayTune::pause; i++) {
                    _delay_ms(1);
                }
            }
            turnOn();
            switch(timer_) {
                case TIMER0:
                    OCR0A=cur_note;
                    break;
                case TIMER1:
#if defined(ATMEGA) || defined(AT90USB)
                    OCR1A=cur_note;
#elif defined(ATTINY)
                    OCR1C=cur_note;
#else
    #error No target for timer1 setup
#endif
                    break;
#if defined(ATMEGA) || defined(AT90USB)

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
            TCCR0B &= ~(1<<CS02) & ~(1<<CS01) & ~(1<<CS00);
            break;
        case TIMER1:
#if defined(ATMEGA) || defined(AT90USB)
            TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10); 
#elif defined(ATTINY)
            TCCR1 &= ~(1<<CS10) & ~(1<<CS11) & ~(1<<CS12) & ~(1<<CS13);
#else
    #error No target for timer1 setup
#endif
            break;
#if defined(ATMEGA) || defined(AT90USB)
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
#if defined(ATMEGA) || defined(ATTINY) || defined(AT90USB)
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
#if defined(ATMEGA) || defined(AT90USB)
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
                case PS_2:
                    TCCR1 |= (1<<CS11);
                    break;
                case PS_4:
                    TCCR1 |= (1<<CS10) | (1<<CS11);
                    break;
                case PS_8:
                    TCCR1 |= (1<<CS12);
                    break;
                case PS_16:
                    TCCR1 |= (1<<CS12);
                    break;
                case PS_32:
                    TCCR1 |= (1<<CS12) | (1<<CS11);
                    break;
                case PS_64:
                    TCCR1 |= (1<<CS12) | (1<<CS11) | (1<<CS10);
                    break;
                case PS_128:
                    TCCR1 |= (1<<CS13);
                    break;
                case PS_256:
                    TCCR1 |= (1<<CS13) | (1<<CS10);
                    break;
                case PS_512:
                    TCCR1 |= (1<<CS13) | (1<<CS11);
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
#if defined(ATMEGA) || defined(AT90USB)
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
#if defined(ATMEGA) || defined(AT90USB)
ISR(TIMER1_COMPB_vect)
{
    TCNT1=0;
}
#endif

    // -- TIMER0
    //set_output(DDRD, PORTD6); 
    //TCCR0A |= (1<<WGM01);  // configure timer 1 for CTC mode
    //TCCR0A |= (1<<COM0A0); // toggle OC0A (PORTD6/arduino 6) on compare match
    //TCCR0B |= (1<<CS02);   // clk/256 prescale
    //OCR0A = 100;

    // -- TIMER1
    //set_output(DDRB, PORTB1);
    //TCCR1B |= (1<<WGM12); // CTC 
    //TCCR1A |= (1<<COM1A0); // toggle OC1A (PORTB1/arduino 9) on compare match
    //TCCR1B |= (1<<CS12); // /256 prescale
    //OCR1A = 100;

    // -- TIMER2
    //set_output(DDRB, PORTB3);
    //TCCR2A |= (1<<COM2A0); // toggle OC2A (PORTB3/arduino 11) on compare match
    //TCCR2A |= (1<<WGM21); // CTC
    //TCCR2B |= (1<<CS22) | (1<<CS21); // clk/256 prescale
    //OCR2A = 100;



