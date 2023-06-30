#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h"


#define CLICK (1 << 6)

int main(void){
  // this initializes the printf/uart thingies
  printf_init(); 

  //connect right click to pin12 and left click to pin9

  //set them both as input 
  DDRB &= ~CLICK;
  DDRH &= ~CLICK;

  //enable pullup resistor
  PORTB |= CLICK;
  PORTH |= CLICK;

  printf("Arduino Serial Mouse Pogram \n");

  while(1){

    if ( (PINB & CLICK) == 0) {
      printf("Right click from pin: %02x \n", (int) PINB);
      _delay_ms(10000);
    }

    if ( (PINH & CLICK) == 0) {
      printf("Left click from pin: %02x \n", (int) PINH);
      _delay_ms(10000);
    }
  }
}