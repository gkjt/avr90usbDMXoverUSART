#include "dmx_universe.h"

/* 1 indexed address numvber */
uint8_t dmx_get_value(DMXUniverse* universe, uint16_t address){
    return universe->data[address-1];
}

/* 1 indexed address numvber */
void dmx_set_value(DMXUniverse* universe, uint16_t address, uint8_t value){
    if(address == 0 || address > 512){
        return;
    }

    if(value > 254){
        universe->data[address-1] = 254;
        return;
    }

    universe->data[address-1] = value;
}
