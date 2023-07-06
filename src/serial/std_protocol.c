#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <stdint.h>

#define BAUD_RATE B1200

int main() {
    int serial_fd = open("/dev/ttyACM0", O_RDWR);
    if(serial_fd  <= 0) {
        printf("Error (Open)\n");
        return 0;
    }
    printf("/dev/ttyACM0 opened \n");

    tcflush(serial_fd, TCIFLUSH);
    struct termios tty;
    tcgetattr(serial_fd, &tty); // setting stored to tty
    cfsetispeed(&tty, BAUD_RATE); // set baudrate input
    cfsetospeed(&tty, BAUD_RATE);
    tty.c_cflag |= (CLOCAL | CREAD); //terminal operated in local mode and enable char receiving
    tty.c_cflag &= ~PARENB; // no parity
    tty.c_cflag &= ~CSTOPB; //one stop bit
    tty.c_cflag &= ~CSIZE; //clear data size bits
    tty.c_cflag |= CS7; // 7 data bits
    tcsetattr(serial_fd, TCSANOW, &tty); // apply tty to serial port

    uint8_t buffer[3];
    int bytes_read;

    while(1) {
        printf("read\n");
        bytes_read = read(serial_fd, buffer, sizeof(buffer));
        printf("postread\n");
        if(bytes_read == -1) {
            printf("Error (read)\n");
            break;
        }
        printf("read \n");
        if(bytes_read == 3 ) {

            printf("Read Completed \n");
            printf("buffer 0: %d\n", buffer[0]);
            printf("buffer 1: %d\n", buffer[1]);
            printf("buffer 2: %d\n", buffer[2]);

            uint8_t lb = (buffer[0] & 0x20); // 0010 0000
            uint8_t rb = (buffer[0] & 0x10); // 0001 0000
            
            int8_t dx = 0;// (buffer[1] & 0x3F); // 0011 1111
            int8_t dy = 0;// (buffer[2] & 0x3F); // 0011 1111

            if( dx != 0 || dy != 0){
                printf("Movement: dx = %d , dy = %d \n", dx,dy);
            }
            
            if(lb != 0) {
                printf("Left Button pressed: %d \n", lb); //32
            }

            if(rb != 0) {
                printf("Right Button pressed: %d \n", rb); //16
            }
        else if (bytes_read == 0) {
            printf("No data sleep");
            usleep(10000);
        }
        }
    }

    close(serial_fd);
    return 0;
}