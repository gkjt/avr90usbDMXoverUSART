/* Ported from github.com/mathertel/DMXSerial */
/* Only the code for sending DMX over USART */
/* Untested on DMX fixtures due to availability */

#include "dmx_usart.h"
/* #define DEBUG 1 */

DMXUniverse* dataUniverse;
int channel = 0;

void init_USART(DMXUniverse* u){
    dataUniverse = u;

    UCSR1B = (1<<TXEN1)|(1<<TXCIE1);
    set_baud(WAITBAUD);
    UDR1 = 0;
}

void set_baud(unsigned long baud){
    /* Set most significant bits of baud rate */
    unsigned long baudrate = ((F_CPU)/(baud*16UL)-1);

    UCSR1A = 0;
    UBRR1H = (unsigned char)(baudrate>>8);
    /* Set least significant bits of baud rate */
    UBRR1L = (unsigned char)baudrate;

    if(baud == WAITBAUD){
        UCSR1C = WAITFORMAT;
    } else if(baud == DMXBAUD){
        UCSR1C = DMXFORMAT;
    }
}

/* ------- Code for SENDING DMX --------- */

void start_sending_dmx(void){
    #ifdef LED_INIT
    LED_INIT;
    #endif

    channel = 0;

    set_baud(WAITBAUD);

    /* Enable send complete ISR */
    UCSR1B |= (1<<TXCIE1) | (1<<UDRIE1); /* DS p.195*/

    /* send break */
    UDR1 = 0;
}

void stop_sending_dmx(){
    UCSR1B = 0;
}

/* USART TX buffer completed sending interrupt */
ISR(USART1_UDRE_vect){
    #ifdef DEBUG
    printf("UDR Written: %d at %d\n", channel, dataUniverse->data[channel]);
    #endif


    UDR1 = dataUniverse->data[channel];
    channel++;

    if(channel > MAX_CHANNEL - 1){
        channel = -1;
        UCSR1B = (1<<TXEN1) | (1<<TXCIE1);

        #ifdef DEBUG
        display_string("Finished Packet\n");
        #endif
    }
}

/* USART TX buffer completed sending interrupt */
/* DMX Packet done! Woooooo! Now do it again. */
ISR(USART1_TX_vect){
    #ifdef LED_ON
    LED_ON;
    #endif
    if(channel == -1){
        set_baud(WAITBAUD);
        UDR1 = 0;
        channel = 0;

        #ifdef DEBUG
        display_string("Wait\n");
        #endif
    } else if (channel == 0){
        set_baud(DMXBAUD);
        UCSR1B = (1<<TXCIE1) | (1<<UDRIE1);
        UDR1 = 0;
        channel = 1;

        #ifdef DEBUG
        display_string("Mark\n");
        #endif
    }

    #ifdef LED_OFF
    LED_OFF;
    #endif
}
