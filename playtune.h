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



/*
   PlayTune, sets up a timer in CTC mode and
   reading frequency and delay information to
   play a tune

   john@jarv.org  2011/11/06

*/
#ifndef PlayTune_h
#define PlayTune_h

#include <stdint.h>
#include <avr/pgmspace.h>

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

#define PS_1024 1024
#define PS_512 512
#define PS_256 256
#define PS_128 128
#define PS_64 64
#define PS_32 32
#define PS_16 16
#define PS_8 8
#define PS_4 4
#define PS_2 2
#define PS_1 1

static const uint8_t def_notes_[] PROGMEM = {118,105,94,88,79,70,62,59};
static const uint8_t def_delays_[] PROGMEM = {1,1,1,1,1,1,1};

class PlayTune
{
    public:
        PlayTune( uint8_t timer, uint16_t prescale=PS_256, 
                const uint8_t *notes=def_notes_,
                const uint8_t *delays=def_delays_,
                uint16_t notes_len=8, uint16_t delays_len=8); 
        uint8_t playNote(void);
        uint8_t turnOff(void);
        uint8_t turnOn(void);
        uint8_t isFinished_;
        uint8_t isPlaying(void);
        static uint16_t pause;

    private:
        const uint8_t *notes_;
        uint16_t notes_len_;
        const uint8_t *delays_;
        uint16_t delays_len_;
        uint8_t timer_; 
        uint16_t prescale_;
        uint8_t timeleft_;
        uint16_t pos_;
        uint8_t last_note_;


};


#endif
