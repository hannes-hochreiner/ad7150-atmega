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
#include <util/delay.h>
#include "twi.h"
#include "sermon.h"

#define CHECK(MSG, FUN) printf("%s...%s\n", MSG, FUN == 0 ? "OK" : "error");

int main() {
  // safety delay
  _delay_ms(3000);

  sermon_setup_4800_8n1();

  printf("initializing...\n");

  char address = 0x90;
  char subAddrVal = 0x01;
  char subAddrRng = 0x0B;

  while (1) {
    char val[2];
    char rng;

    printf("reading value...\n");
    CHECK("enabled", twi_enable())
    CHECK("started", twi_start())
    CHECK("written", twi_write(&address, &subAddrVal))
    CHECK("restarted", twi_start())
    CHECK("read", twi_read(&address, val, 2))
    CHECK("started", twi_start())
    CHECK("written", twi_write(&address, &subAddrRng))
    CHECK("restarted", twi_start())
    CHECK("read", twi_read(&address, &rng, 1))
    CHECK("stopped", twi_stop())
    CHECK("disabled", twi_disable())

    unsigned int intVal = ((val[0] << 8) | val[1]);

    printf("value: %u; range: %u\n", intVal, rng >> 6);
    _delay_ms(500);
  }

  return 0;
}
