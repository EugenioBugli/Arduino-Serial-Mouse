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
  printf("%u\n", data);
}

void Send_packets(uint8_t lb, uint8_t rb, int8_t dy, int8_t dx) {
  uint8_t first_pack = 0x80 | (lb << 5) | (rb << 4) ;//| ( (dy >> 6) & 0x03 ) | ( (dx >> 6) );
  uint8_t second_pack = 0x00;//dx & 0x7F;
  uint8_t third_pack = 0x00;//dy & 0x7F;
  //printf("Send First\n");
  UART_transmit(first_pack);
  //_delay_ms(2000);
  //printf("Send Second\n");
  UART_transmit(second_pack);
  //_delay_ms(2000);
  //printf("Send Third\n");
  UART_transmit(third_pack);
  //_delay_ms(2000);
  //printf("packets sent\n");
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

  
  int8_t X_counter = 0;
  int8_t Y_counter = 0;

  while(1){

    /*if ( (PINB & CLICK) == 0) {
      //printf("Right click from pin: %02x \n", (int) PINB);
      //_delay_ms(1000);
    }

    if ( (PINH & CLICK) == 0) {
      //printf("Left click from pin: %02x \n", (int) PINH);
      //_delay_ms(1000);
    }*/

    if ( (PINF & MOVE) == 0) {
      //printf("Moving forward. Y position: %d\n", Y_counter);
      Y_counter++;
      _delay_ms(1000);
    }

    if ( (PINL & MOVE ) == 0) {
      //printf("Moving right. X position: %d\n", X_counter);
      X_counter++;
      _delay_ms(1000);
    }

    if ( (PINK & MOVE) == 0) {
      //printf("Moving backward. Y position: %d\n", Y_counter);
      Y_counter--;
      _delay_ms(1000);
    }

    if ( (PING & MOVE ) == 0) {
      //printf("Moving left. X position: %d\n", X_counter);
      X_counter--;
      _delay_ms(1000);
    }
    //printf("Actual State : \nRight Click: %d\nLeft Click: %d\nX: %d\nY: %d\n\n",(PINB & CLICK) == 0, (PINH & CLICK) == 0, X_counter, Y_counter);
    uint8_t lb = (PINH & CLICK) == 0;
    uint8_t rb = (PINB & CLICK) == 0;
    Send_packets(lb, rb, Y_counter, X_counter);
  }
}