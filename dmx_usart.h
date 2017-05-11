#include "dmx_universe.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "lcd.h"

#ifndef _DMXUSART_
#define _DMXUSART_
#define F_CPU 8000000UL
#define DMXBAUD  250000ul  // set baud rate value for normal transmission
#define WAITBAUD 100000ul  // set baud rate value for breaks
#define MAX_CHANNEL 512
#define DMXFORMAT  ((1<<USBS1) | (0<<UPM10) | (3<<UCSZ10))
#define WAITFORMAT ((0<<USBS1) | (2<<UPM10) | (3<<UCSZ10))

/* #define LED_INIT    DDRB  |=  _BV(PINB7) */
/* #define LED_ON      PORTB |=  _BV(PINB7) */
/* #define LED_OFF     PORTB &= ~_BV(PINB7) */


void init_USART(DMXUniverse* u);
void set_baud(unsigned long baud);
void start_sending_dmx(void);
void stop_sending_dmx(void);



#endif
