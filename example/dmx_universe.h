#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef _DMXU_
#define _DMXU_
struct dmx_universe{uint8_t data[512];};
typedef struct dmx_universe DMXUniverse;

uint8_t dmx_get_value(DMXUniverse* universe, uint16_t address);
void dmx_set_value(DMXUniverse* universe, uint16_t address, uint8_t value);
#endif
