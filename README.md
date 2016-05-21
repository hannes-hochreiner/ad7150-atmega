# AD7150 ATmega
Interfacing the AD7150 capacitance converter with an ATmega mcu.

## Wiring
The AD7150 uses an [I2C interface](https://en.wikipedia.org/wiki/I%C2%B2C) at a maximum of 400 kHz.
The ATmega328p also supports a two-wire interface at this speed.
However, sine the MCU will run 1 MHz, the maximum speed of the TWI is about 60 kHz.
Since the internal pull-ups of the Atmega328p are not enough, external 1.5 kOhm pull-ups were used.
