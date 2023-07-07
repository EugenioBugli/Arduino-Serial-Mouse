#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <stdint.h>

#define BAUD_RATE B1200

int main() {
    int serial_fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if(serial_fd  <= 0) {
        printf("Error (Open)\n");
        return 0;
    }
    printf("/dev/ttyACM0 opened \n");
    usleep(100000);
    printf("waited\n");
    tcflush(serial_fd, TCIFLUSH);
    struct termios tty;
    tcgetattr(serial_fd, &tty); // setting stored to tty
    cfsetispeed(&tty, BAUD_RATE); // set baudrate input
    cfsetospeed(&tty, BAUD_RATE);
    tty.c_cflag |= (CLOCAL | CREAD); //terminal operated in local mode and enable char receiving
    //tty.c_cflag &= ~PARENB; // no parity
    //tty.c_cflag &= ~CSTOPB; //one stop bit
    //tty.c_cflag &= ~CSIZE; //clear data size bits
    //tty.c_cflag |= CS7; // 7 data bits
    tcsetattr(serial_fd, TCSANOW, &tty); // apply tty to serial port

    uint8_t buffer[3];
    ssize_t bytes_read;
    int idx = 0;
    while(1) {
        bytes_read = read(serial_fd, &buffer[idx], sizeof(buffer[idx]));
        if(bytes_read == -1) {
            printf("Error during Read\n");
            break;
        }
        if(bytes_read > 0) {
            // check data
            if((buffer[0] & 0x80) && !(buffer[1] & 0x80) && !(buffer[2] & 0x80) ) {
                uint8_t first = buffer[0];
                printf("First packet: %u\n", first);
                uint8_t second = buffer[1];
                printf("Second packet: %u\n", second);
                uint8_t third = buffer[2];
                printf("Third packet: %u\n", third);
                idx = 0;
                uint8_t lb = first & 0x10;
                uint8_t rb = first & 0x20;
                uint8_t dx = second & 0x3F;
                uint8_t dy = third & 0x3F;
                if(lb) printf("Left click\n");
                if(rb) printf("Right Click\n");
                printf("current position: %u,%u\n ", dx, dy);
                printf("\n------\n");
            }
        }
    }
    

    close(serial_fd);
    return 0;
}