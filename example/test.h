#include "dmx_universe.h"
#include "dmx_usart.h"
#include "switches.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define MIN_STEP    -100    /* > 0 */
#define MAX_STEP  100


void init(void);
int main(void);
int8_t enc_delta();
