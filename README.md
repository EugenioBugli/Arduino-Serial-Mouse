# Arduino-Serial-Mouse

Sistemi Operativi

Prof: Giorgio Grisetti

Based on : https://roborooter.com/post/serial-mice/ and https://linux.die.net/man/4/mouse

avr_common folder author : Giorgio Grisetti

A3. Arduino serial mouse
   Implement a fake mouse, simulated with 6 keys: up, down , left, right and left/right button.
   The fake mouse connects to the serial port, and sends the "values", according to a serial mouse
   protocol at your choice.
   Once this is done and the protocol verified, you can connect it to the X server following these guide:
   https://help.ubuntu.com/community/SerialMouseHowto

ATMEGA2560

UP: pin A1 (bit 1 port F) 

DOWN: pin A9 (bit 1 port K)

LEFT: pin 40 (bit 1 port G)

RIGHT: pin 48 (bit 1 port L)

LEFT CLICK: pin 12 (bit 6 port B)

RIGHT CLICK: pin 9 (bit 6 port H)

Serial Mouse Protocol Used : Microsoft Protocol
