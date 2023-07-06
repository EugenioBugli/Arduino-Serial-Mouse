#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

#define BAUD_RATE B1200

int main() {
    int serial_fd = open("/dev/ttyACM0", O_RDONLY);
    if(serial_fd  <= 0) {
        printf("Error (Open)\n");
        return 0;
    }
    printf("/dev/ttyACM0 opened \n");

    tcflush(serial_fd, TCIFLUSH);
    struct termios tty;
    tcgetattr(serial_fd, &tty); // setting stored to tty
    cfsetispeed(&tty, BAUD_RATE); // set baudrate input
    cfsetospeed(&tty, BAUD_RATE); //set baudrate output
    tcsetattr(serial_fd, TCSANOW, &tty); // apply tty to serial port

    int buffer[3];
    int bytes_read;

    while(1) {

        bytes_read = read(serial_fd, buffer, sizeof(buffer));
        if(bytes_read == -1) {
            printf("Error (read)\n");
            break;
        }

        if(bytes_read == 3 && (buffer[0] & 0x08)) {
            int dx = (buffer[0] & 0x3F); // 0011 1111
            int dy = (buffer[1] & 0x3F); // 0011 1111
            int lb = (buffer[0] & 0x20); // 0010 0000
            int rb = (buffer[0] & 0x10); // 0001 0000

            if( dx != 0 || dy != 0){
                printf("Movement: dx = %d , dy = %d \n", dx,dy);
            }
            
            if(lb != 0) {
                printf("Left Button pressed: %d \n", lb); //32
            }

            if(rb != 0) {
                printf("Right Button pressed: %d \n", rb); //16
            }
        }
    }

    close(serial_fd);
    return 0;
}