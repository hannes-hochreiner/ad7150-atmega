// The MIT License (MIT)
//
// Copyright (c) 2016 Hannes Hochreiner
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "twi.h"

int twi_enable() {
  // switch on the twi clock
  PRR &= ~(1<<PRTWI);
  // set bit rate
  TWBR = 0x00;
  // set prescaler
  TWSR = 0x00;
  // enable twi
  TWCR = (1<<TWEN);

  return 0;
}

int twi_disable() {
  // disable twi
  TWCR = 0x00;

  return 0;
}

int twi_start() {
  // send start condition
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
  // wait for completion flag
  while (!(TWCR & (1<<TWINT)));
  // check twi status register
  return ((TWSR & 0xF8) != TWI_START) && ((TWSR & 0xF8) != TWI_RESTART) ? -1 : 0;
}

int twi_read(const char const* address, char* data, int length) {
  // set slave address plus read
  TWDR = *address | 0x01;
  // send address
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for completion flag
  while (!(TWCR & (1<<TWINT)));
  // check twi status register
  if ((TWSR & 0xF8) != TWI_SLAR_ACK) {
    return -1;
  }

  for (int cntr = 0; cntr < length - 1; cntr++) {
    // receive data
    TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
    // wait for completion flag
    while (!(TWCR & (1<<TWINT)));
    // check twi status register
    if ((TWSR & 0xF8) != TWI_DATAR_ACK) {
      return -1;
    }

    *(data + cntr) = TWDR;
  }

  // receive last byte
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for completion flag
  while (!(TWCR & (1<<TWINT)));
  // check twi status register
  if ((TWSR & 0xF8) != TWI_DATAR_NACK) {
    return -1;
  }

  *(data + length - 1) = TWDR;

  return 0;
}

int twi_write(const char const* address, const char const* data) {
  // set slave address plus write
  TWDR = *address & ~0x01;
  // send address
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for completion flag
  while (!(TWCR & (1<<TWINT)));
  // check twi status register
  if ((TWSR & 0xF8) != TWI_SLAW_ACK) {
    return -1;
  }
  // set data
  TWDR = *data;
  // send data
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for completion flag
  while (!(TWCR & (1<<TWINT)));
  // check twi status register
  return (TWSR & 0xF8) != TWI_DATAW_ACK ? -1 : 0;
}

int twi_stop() {
  // send start condition
  TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
  // completion flag will not be set

  return 0;
}
