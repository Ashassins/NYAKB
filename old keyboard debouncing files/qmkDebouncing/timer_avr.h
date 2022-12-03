
#include <stdint.h>

#ifndef TIMER_PRESCALER
#    if F_CPU > 16000000
#        define TIMER_PRESCALER 256
#    elif F_CPU > 2000000
#        define TIMER_PRESCALER 64
#    elif F_CPU > 250000
#        define TIMER_PRESCALER 8
#    else
#        define TIMER_PRESCALER 1
#    endif
#endif
#define TIMER_RAW_FREQ (F_CPU / TIMER_PRESCALER)
#define TIMER_RAW TCNT0
#define TIMER_RAW_TOP (TIMER_RAW_FREQ / 1000)

#if (TIMER_RAW_TOP > 255)
#    error "Timer0 can't count 1ms at this clock freq. Use larger prescaler."
#endif
