/*
__author__ = "Karol Celinski"
__copyright__ = "Copyright 2017, Immunity Systems Sp. z o.o."
__credits__ = ["Andrzej Nowodworski", "Pawel Maziarz"]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Karol Celinski"
__email__ = "karolc@immunity-systems.com"
 */

#include "HiDude.h"
#include <avr/pgmspace.h>

/*
 * ABDDDD
 * A - Repeat payload - 0 or 1
 * B - Sleep before start - 0 or 1
 * D - number of miliseconds to sleep
 */
const char cnf_eeprom[] PROGMEM = "CONFXX";
char cnf_ram[2];

const char payload[] PROGMEM = 
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOADPAYLOAD"
                "PAYLOADPAYLOAD"
                ;

unsigned short idx = 0;
byte c=0;

void setup() {
  idx=1;
  c=0;
  c = pgm_read_byte(&payload[0]);
  *((unsigned short*)cnf_ram) = pgm_read_word(&cnf_eeprom[0]);
  unsigned long t = pgm_read_dword(&cnf_eeprom[2]);
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A 
  digitalWrite(1, HIGH);
  digitalWrite(0, HIGH);
  if (cnf_ram[1]) {
    HiDude.delay( t );
  }
  HiDude.sendKeyPress(0,0,0);   
}

void loop() {
  HiDude.update();
  if (c==0x8e) {
        unsigned long t = pgm_read_dword(&payload[idx]);
        HiDude.delay(t);
        idx+=4;
        
  } else if(c>0xf0){
        HiDude.update();
        char buf[3] = {0,0,0};
        byte i;
        for(i=0; i<c-0xf0; i++) {
          buf[i]=pgm_read_byte(&payload[idx+i]);
          HiDude.update();
        }
        idx+=i;
        HiDude.sendKeyPress(buf[0],buf[1],buf[2]);
        HiDude.sendKeyPress(0,0,0);
  } else if(c>=0x8f && c<=0x91) {
    uchar buf[4] = {0,0,0,0};
    if(c == 0x8f)
      buf[0]=pgm_read_byte(&payload[idx]);
    else if(c ==0x90) {
      buf[ 1 ]=pgm_read_byte(&payload[idx]);
      buf[ 2 ]=pgm_read_byte(&payload[idx+1]);
      idx+=1;
    } else if(c ==0x91) {
      buf[ 3 ]=pgm_read_byte(&payload[idx]);
    }
    HiDude.mouse(buf[0],buf[1],buf[2],buf[3]);
    idx++;
  } else {
    HiDude.sendKeyPress(c,0,0);
    HiDude.sendKeyPress(0,0,0);
  }
  c = pgm_read_byte(&payload[idx]);
  if (c==0) {
    HiDude.update();
    idx=0;
    c = pgm_read_byte(&payload[idx++]);
    HiDude.update();
    if (!cnf_ram[0]) {
      while(1) {
        //nothing loop ;-)
      }
    }
  } else 
    idx++;
}
