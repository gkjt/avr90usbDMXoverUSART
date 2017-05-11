# avr90usbDMXoverUSART
A library for outputting DMX-512 over USART on the AVR90USB1286

Use by including the two files. If you only want a DMX universe datastructure then feel free to only include dmx_universe.h

To output DMX:
```
DMXUniverse u;
init_USART(&u);
start_sending_dmx();
...
stop_sending_dmx();
```

To use values in the universe:
```
void dmx_set_value(DMXUniverse* universe, uint16_t address, uint8_t value);
uint8_t dmx_get_value(DMXUniverse* universe, uint16_t address);
```
