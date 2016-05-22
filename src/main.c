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

#define CHECK(MSG, FUN) printf("%s", FUN == 0 ? "O" : "X");

int main() {
  // safety delay
  _delay_ms(3000);

  sermon_setup_4800_8n1();

  printf("initializing...");

  char address = 0x90;
  char subAddrVal = 0x01;
  char subAddrSetup1 = 0x0B;
  char subAddrSetup2 = 0x0E;
  char subAddrCap = 0x11;
  char subAddrStatus = 0x00;

  char valConf[] = {0x0F, (1<<3) | 1};
  char ch2Conf[] = {subAddrSetup2, 0x8B};

  CHECK("enabled", twi_enable())
  CHECK("started", twi_start())
  CHECK("written", twi_write(&address, valConf, 2))
  CHECK("written", twi_write(&address, ch2Conf, 2))
  CHECK("stopped", twi_stop())
  // CHECK("disabled", twi_disable())

  printf("\n");

  while (1) {
    char conf[] = {CONFR, 0b00001010};
    char val[8];
    char cap[2];
    char stat;

    // CHECK("enabled", twi_enable())
    CHECK("started", twi_start())
    CHECK("written", twi_write(&address, &subAddrStatus, 1))
    CHECK("restarted", twi_start())
    CHECK("read", twi_read(&address, &stat, 1))
    CHECK("restarted", twi_start())
    CHECK("written", twi_write(&address, &subAddrVal, 1))
    CHECK("restarted", twi_start())
    CHECK("read", twi_read(&address, val, 8))
    CHECK("started", twi_start())
    CHECK("written", twi_write(&address, &subAddrCap, 1))
    CHECK("restarted", twi_start())
    CHECK("read", twi_read(&address, cap, 2))
    CHECK("stopped", twi_stop())
    // CHECK("disabled", twi_disable())

    unsigned int intVal1 = ((val[0] << 8) | val[1]);
    unsigned int intVal2 = ((val[2] << 8) | val[3]);
    unsigned int intAvg1 = ((val[4] << 8) | val[5]);
    unsigned int intAvg2 = ((val[6] << 8) | val[7]);

    printf(" stat: %x, val1: %u; avg1: %u; cap1: %u; val2: %u; avg2: %u; cap2: %u\n", stat, intVal1, intAvg1, cap[0] & 0x3F, intVal2, intAvg2, cap[1] & 0x3F);

    _delay_ms(200);
  }

  return 0;
}
