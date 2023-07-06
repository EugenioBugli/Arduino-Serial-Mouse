#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h"


#define CLICK (1 << 6)
#define MOVE (1 << 1)
#define BAUD 1200
#define MYUBRR (F_CPU/16/BAUD-1)

//using int8_t instead of uint8_t to store negative numbers ( range : -128 : 127 )
int8_t X_counter = 0;
int8_t Y_counter = 0;

void UART_init(void) {
  //set baud rate
  UBRR0H = (uint8_t) ( MYUBRR >> 8 );
  UBRR0L = (uint8_t) MYUBRR;
  //enable transmission
  UCSR0B = (1 << TXEN0);
  //set frame format (7 data bits + 1 stop bit)
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit(uint8_t data) {
  while( !(UCSR0A & (1 << UDRE0)) ); //polling ---> wait for transmission completed
  UDR0 = data; //transmit
}

int main(void){
  // this initializes the printf/uart thingies
  printf_init(); 
  UART_init();
  //connect right click to pin12 and left click to pin9

  //set them both as input 
  DDRB &= ~CLICK;
  DDRH &= ~CLICK;

  //enable pullup resistor
  PORTB |= CLICK;
  PORTH |= CLICK;

  //set y :

  //set both as input
  DDRF &= ~MOVE; // pin A1 : UP
  DDRK &= ~MOVE; // pin A9 : DOWN

  //enable pullup resistor
  PORTF |= MOVE;
  PORTK |= MOVE;

  //set x :

  //set both as input
  DDRL &= ~MOVE; // pin 48 : RIGHT
  DDRG &= ~MOVE; // pin 40 : LEFT

  //enable pullup resistor
  PORTL |= MOVE;
  PORTG |= MOVE;

  //printf("\n");
  //printf("Initial coordinates: (%d,%d) \n", X_counter, Y_counter);
  //printf("\n");
  while(1){
    uint8_t first = (1 << 7);
    uint8_t second = 0x0;
    uint8_t third = 0x0;

    if ( (PINB & CLICK) == 0) {
      //printf("Right click from pin: %02x \n", (int) PINB);
      first = first | (1 << 4);
      //_delay_ms(1000);
    }

    if ( (PINH & CLICK) == 0) {
      //printf("Left click from pin: %02x \n", (int) PINH);
      first = first | (1 << 5);
      //_delay_ms(1000);
    }

    if ( (PINF & MOVE) == 0) {
      //printf("Moving forward. Y position: %d\n", Y_counter);
      Y_counter++;
      //_delay_ms(1000);
    }

    if ( (PINL & MOVE ) == 0) {
      //printf("Moving right. X position: %d\n", X_counter);
      X_counter++;
      //_delay_ms(1000);
    }

    if ( (PINK & MOVE) == 0) {
      //printf("Moving backward. Y position: %d\n", Y_counter);
      Y_counter--;
      //_delay_ms(1000);
    }

    if ( (PING & MOVE ) == 0) {
      //printf("Moving left. X position: %d\n", X_counter);
      X_counter--;
      //_delay_ms(1000);
    }

    printf("Actual State : \nRight Click: %d\nLeft Click: %d\nX: %d\nY: %d\n\n",(PINB & CLICK) == 0, (PINH & CLICK) == 0, X_counter, Y_counter);
    UART_transmit(first);
    UART_transmit(second);
    UART_transmit(third);
    _delay_ms(8000);
  }
}