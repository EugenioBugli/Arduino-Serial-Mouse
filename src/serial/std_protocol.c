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

    uint8_t buffer[14];
    ssize_t bytes_read;
    ssize_t total;
    uint8_t first, second, third;
    int idx = 0;
    while(1) {
        bytes_read = read(serial_fd, &buffer[idx], sizeof(buffer[idx]));
        if(bytes_read == -1) {
            printf("Error during Read\n");
            break;
        }
        if(total == sizeof(buffer[idx])*14) {
            //check pack.txt
            first = buffer[0];
            second = buffer[6];
            third = buffer[10];
            //check bit 6 of data packs
            if ((first & 0x80) && !(second & 0x80) && !(third & 0x80)) {
                printf("First packet: %u\n", first);
                printf("Second packet: %u\n", second);
                printf("Third packet: %u\n", third);

                uint8_t lb = first & 0x20;
                uint8_t rb = first & 0x10;
                int8_t dx = (second & 0x3F) | (first & 0x03);
                int8_t dy = (third & 0x3F) | (first & 0xC0);

                if (lb) printf("Left Click \n");
                if (rb) printf("Right Click \n");

                printf("Actual Coordinates: %d,%d \n", dx, dy);
                printf("-----------\n");
            }
            idx = 0;
            total = 0;
        }
        else {
            idx++;
            total += bytes_read;
        }
    }
    

    close(serial_fd);
    return 0;
}