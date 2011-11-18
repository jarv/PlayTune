/*
   PlayTune, sets up a timer in CTC mode and
   reading frequency and delay information to
   play a tune

   john@jarv.org  2011/11/06

*/
#ifndef PlayTune_h
#define PlayTune_h

#include <stdint.h>

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

#define PS_1024 1024
#define PS_256 256
#define PS_64 64
#define PS_8 8
#define PS_1 1


class PlayTune
{
    public:
        PlayTune( uint8_t timer, uint16_t prescale, 
                const uint8_t *notes=0, const uint8_t *delays=0,
                uint16_t notes_len=0, uint16_t delays_len=0); 
        uint8_t playNote(void);
        uint8_t turnOff(void);
        uint8_t turnOn(void);
        static uint16_t pause;

    private:
        uint8_t isFinished;
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
