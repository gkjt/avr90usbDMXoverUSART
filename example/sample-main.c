
#include "sample-main.h"

/* Get a universe of DMZ values */
DMXUniverse u;
int selectedChannel=1;
int currentLevel;
int delta;

void init(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
    init_switches();

    /* Initialise USART DMX */
    init_USART(&u);
}



int main(){
    init();
    int i;
    /* Initialise the DMX universe datastructure */
    for(i=0; i<512; i++)
        u.data[i]=0;
    sei();

    /* Start Sending DMX */
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
