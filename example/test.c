
#include "test.h"

DMXUniverse u;
int selectedChannel=1;
int currentLevel;
int delta;

void init(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    /* ENABLE ENCODER INPUTS AND PULL-UPS */
    DDRE  &= ~_BV(PE4);
    DDRE  &= ~_BV(PE5);
    PORTE |=  _BV(PORTE4);
    PORTE |=  _BV(PORTE5);

	/* Timer 0 for switch scan interrupt: */

	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01)
          | _BV(CS00);   /* F_CPU / 64 */


    /* SET OCR0A FOR A 1 MS PERIOD */
    OCR0A = 125;
    /* ENABLE TIMER INTERRUPT */
    TIMSK0 |= _BV(OCIE0A);

    init_lcd();
    init_USART(&u);
    init_switches();
}

ISR( TIMER0_COMPA_vect ) {
    static int8_t last;
    int8_t new, diff;
    uint8_t wheel;


    /*
       Scan rotary encoder
       ===================
       This is adapted from Peter Dannegger's code available at:
       http://www.mikrocontroller.net/articles/Drehgeber
    */

    wheel = PINE;
    new = 0;
    if( wheel  & _BV(PE4) ) new = 3;
    if( wheel  & _BV(PE5) )
    new ^= 1;		        	/* convert gray to binary */
    diff = last - new;			/* difference last - new  */
    if( diff & 1 ){			/* bit 0 = value (1) */
        last = new;		       	/* store new as next last  */
        delta += (diff & 2) - 1;	/* bit 1 = direction (+/-) */
    }

}


/* read two step encoder */
int8_t enc_delta() {
   int8_t val;
   val = delta*5;
   delta &= 1;

   return val >> 1;
}



int main(){
    init();
    int i;
    for(i=0; i<512; i++)
        u.data[i]=0;
    sei();
    start_sending_dmx();
    while(1){
        if(left_pressed()){
            currentLevel+=10;
            if(currentLevel < 0) currentLevel = 0;
            if(currentLevel > 254) currentLevel = 250;
            dmx_set_value(&u,selectedChannel,currentLevel);
        }
        if(right_pressed()){
            currentLevel-=10;
            if(currentLevel < 0) currentLevel = 0;
            if(currentLevel > 254) currentLevel = 250;
            dmx_set_value(&u,selectedChannel,currentLevel);
        }

        if(up_pressed()){
            selectedChannel++;
            if(selectedChannel < 1) selectedChannel = 1;
            if(selectedChannel > 254) selectedChannel = 512;
            currentLevel = dmx_get_value(&u,selectedChannel);
        }
        if(down_pressed()){
            selectedChannel--;
            if(selectedChannel < 1) selectedChannel = 1;
            if(selectedChannel > 254) selectedChannel = 512;
            currentLevel = dmx_get_value(&u,selectedChannel);
        }

        char level[4] = "000";
        char channel[4] = "000";
        sprintf(channel, "%03d", selectedChannel);
        sprintf(level, "%03d", currentLevel);
        display_string_xy(channel, 50, 50);
        display_string_xy("@", 100, 50);
        display_string_xy(level, 150, 50);
    }
}
