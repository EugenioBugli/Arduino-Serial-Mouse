#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <stdint.h>

#define BAUD_RATE B1200

int main() {
    int serial_fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
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
    tty.c_cflag |= CLOCAL | CREAD; // 7 data bits, terminal operated in local mode and enable char receiving
    tty.c_iflag = 0; // disable input processing
    tty.c_oflag = 0; // disable output processing
    tty.c_lflag = 0; // disable line processing
    tcsetattr(serial_fd, TCSANOW, &tty); // apply tty to serial port

    uint8_t buffer[3];
    ssize_t bytes_read;
    uint8_t first, second, third;
    int idx = 0;
    while(1) {
        bytes_read = read(serial_fd, &buffer[idx], sizeof(buffer[idx]));
        if(bytes_read == -1) {
            printf("Error during Read\n");
            break;
        }
        //printf("idx %d : %u\n",idx, buffer[idx]);
        if(idx == (sizeof(buffer)-1) && bytes_read == sizeof(buffer[idx]) ) {
            /*
            for(int i = 0; i < 3; i++) {
                printf("I've read: %u\n", buffer[i]);
            }
            printf("-----------\n");
            */
            first = buffer[0];
            second = buffer[1];
            third = buffer[2];
            printf("First packet: %u\n", first);
            printf("Second packet: %u\n", second);
            printf("Third packet: %u\n", third);
            //check data format
            if ((first & 0x80) && !(second & 0x80) && !(third & 0x80)) {

                uint8_t lb = first & 0x20;
                uint8_t rb = first & 0x10;
                int8_t dx = (second & 0x3F) | (first & 0x03);
                int8_t dy = (third & 0x3F) | (first & 0x0C);

                if (lb) printf("Left Click \n");
                if (rb) printf("Right Click \n");

                printf("Actual Coordinates: %d,%d \n", dx, dy);
                printf("-----------\n");
            }
            idx = 0;
        }
        else {
            idx++;
        }
    }
    

    close(serial_fd);
    return 0;
}