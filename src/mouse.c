#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h"


#define CLICK (1 << 6)
#define X_MOVE (1 << 1)
#define Y_MOVE (1 << 1)

//using int8_t instead of uint8_t to store negative numbers ( range : -128 : 127 )
int8_t X_counter = 0;
int8_t Y_counter = 0;

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

  //set y :

  //set both as input
  DDRF &= ~Y_MOVE; // pin A1 : UP
  DDRK &= ~Y_MOVE; // pin A9 : DOWN

  //enable pullup resistor
  PORTF |= Y_MOVE;
  PORTK |= Y_MOVE;

  //set x :

  //set both as input
  DDRL &= ~X_MOVE; // pin 48 : RIGHT
  DDRG &= ~X_MOVE; // pin 40 : LEFT

  //enable pullup resistor
  PORTL |= X_MOVE;
  PORTG |= X_MOVE;

  printf("\n");
  printf("Arduino Serial Mouse Pogram\n");
  printf("Initial coordinates: (%d,%d) \n", X_counter, Y_counter);
  while(1){

    if ( (PINB & CLICK) == 0) {
      printf("Right click from pin: %02x \n", (int) PINB);
      _delay_ms(10000);
    }

    if ( (PINH & CLICK) == 0) {
      printf("Left click from pin: %02x \n", (int) PINH);
      _delay_ms(10000);
    }

    if ( (PINF & Y_MOVE) == 0) {
      printf("Moving forward. Y position: %d\n", Y_counter);
      Y_counter++;
      _delay_ms(1000);
    }

    if ( (PINL & X_MOVE ) == 0) {
      printf("Moving right. X position: %d\n", X_counter);
      X_counter++;
      _delay_ms(1000);
    }

    if ( (PINK & Y_MOVE) == 0) {
      printf("Moving backward. Y position: %d\n", Y_counter);
      Y_counter--;
      _delay_ms(1000);
    }

    if ( (PING & X_MOVE ) == 0) {
      printf("Moving left. X position: %d\n", X_counter);
      X_counter--;
      _delay_ms(1000);
    }
  }
}