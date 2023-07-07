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

    uint8_t buffer[4];
    ssize_t bytes_read;
    int idx = 0;
    while(1) {
        bytes_read = read(serial_fd, &buffer[idx], sizeof(buffer[idx]));
        if(bytes_read == -1) {
            printf("Error during Read\n");
            break;
        }
        printf("You've read something: %u, where idx is %d\n", buffer[idx], idx);
        if(bytes_read > 0) {
            if(idx <=2 ) idx++;
            else {
                uint8_t first = buffer[0];
                idx = 0;
                printf("\n------\n");
            }
        }
    }
    

    close(serial_fd);
    return 0;
}